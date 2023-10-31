#include "Room.hpp"
#include "../Utils/Scheduling.hpp"
#include <bitset>
#include <cmath>

Room::Room(u_int id, std::shared_ptr<Client> client, Levels &levels, bool privateRoom):
    _levels(levels)
{
    _id = id;
    _playersIds = 0;
    _maxPlayer = 4;
    _progress = 0;
    _lastMapRefresh = 0;
    _state = WAIT;

    _missilesIds = 0;
    _monstersIds = 0;

    addPlayer(client);
    _lastJoin = NOW;
    _private = privateRoom;
    _lastWaitMessage = NOW;
    _thread = std::thread(&Room::refresh, this);
}

Room::~Room()
{
    if (_thread.joinable())
        _thread.join();
}

Room::State Room::getState() const
{
    return _state;
}

size_t Room::getCurrentLevel() const
{
    return _levels.lvl();
}

u_int Room::getId() const
{
    return _id;
}

unsigned int Room::getNbPlayer() const
{
    return _players.size();
}

unsigned int Room::getProgress() const
{
    return _progress;
}

unsigned int Room::getMaxPlayer() const
{
    return _maxPlayer;
}

u_int &Room::getMissilesIds()
{
    return _missilesIds;
}

void Room::addPlayer(std::shared_ptr<Client> client)
{
    std::unique_lock<std::mutex> lock(_playersMutex);
    for (auto i = _players.begin(); i != _players.end(); i++)
        if ((**i).client() == client)
            return;

    u_int newId = ++_playersIds;

    client->send(StreamFactory::joinRoom(_id, newId));

    for (auto i = _players.begin(); i != _players.end(); i++)
        client->send(StreamFactory::playerJoinedGame((**i).id()));

    _players.push_back(std::make_unique<Player>(*this, client, newId, 0, SCREEN_HEIGHT / _maxPlayer * _players.size()));
    _lastJoin = NOW;
}

void Room::removePlayer(std::shared_ptr<Client> client)
{
    std::unique_lock<std::mutex> lock(_playersMutex);

    for (auto i = _players.begin(); i != _players.end(); i++) {
        if ((**i).client() == client) {
            std::cout << "Player " << (**i).id() << " disconnected in room " << _id << std::endl;
            sendToAll(StreamFactory::playerLeftGame((**i).id()));
            _players.erase(i);
            break;
        }
    }
}

void Room::movePlayer(std::shared_ptr<Client> client, char move, char nbr)
{
    auto now = std::chrono::system_clock::now();
    Player &player = getPlayer(client);

    std::unique_lock<std::mutex> lock(_playersMutex);
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - player.lastMoveTime()).count() >= PLAYER_MOVE_TIME) {
        player.setLastMoveTime(now);
        for (int i = 0; i < nbr; i++) {
            if (move & PLAYER_MOVE_UP)
                player.move(0, -PLAYER_PROGRESS_STEP);
            if (move & PLAYER_MOVE_DOWN)
                player.move(0, PLAYER_PROGRESS_STEP);
            if (move & PLAYER_MOVE_LEFT)
                player.move(-PLAYER_PROGRESS_STEP, 0);
            if (move & PLAYER_MOVE_RIGHT)
                player.move(PLAYER_PROGRESS_STEP, 0);
        }
        if (move && nbr) {
            player.sendPos();
        }
    }
}

bool Room::isClientInRoom(std::shared_ptr<Client> client)
{
    std::unique_lock<std::mutex> lock(_playersMutex);

    for (auto i = _players.begin(); i != _players.end(); i++) {
        if ((**i).client() == client) {
            return true;
        }
    }
    return false;
}

Player &Room::getPlayer(std::shared_ptr<Client> client)
{
    std::unique_lock<std::mutex> lock(_playersMutex);

    for (auto i = _players.begin(); i != _players.end(); i++) {
        if ((**i).client() == client) {
            return **i;
        }
    }
    throw std::runtime_error("Player not found");
}

void Room::sendToAll(const Stream &stream)
{
    for (auto i = _players.begin(); i != _players.end(); i++) {
        if (*i)
            (**i).client()->send(stream);
    }
}

void Room::refresh()
{
    while (true) {
        _playersMutex.lock();
        for (auto i = _players.begin(); i != _players.end(); i++) {
            if (!(**i).client()->isAlive()) {
                removePlayer((**i).client());
                break;
            }
        }
        _playersMutex.unlock();
        update();
        if (_players.size() == 0) {
            _state = STOPPED;
            break;
        }
    }
}

void Room::update()
{
    size_t now = NOW;
    if (_state == RUN) {
        if (now - _lastMapRefresh >= MAP_REFRESH_TIME) {
            _lastMapRefresh = now;
            _progress += MAP_PROGRESS_STEP;
            sendToAll(StreamFactory::screenProgress(_progress));
            now = NOW;
        }
        _playersMutex.lock();
        // if (_players.size() == 0) {
        //     _state = END;
        //     _lastGameOver = 0;
        //     return;
        // }
        for (auto i = _players.begin(); i != _players.end(); i++) {
            (**i).refresh();
        }
        for (auto i = _monsters.begin(); i != _monsters.end();) {
            (**i).refresh();
            if ((**i).isDeletable()) {
                _monsters.erase(i);
                continue;
            }
            if ((**i).exists() && (**i).isOutOfScreen())
                (**i).kill();
            else
                i++;
        }

        checkCollisionPlayer();
        checkCollisionMonsters();

        _levels.update(*this);
        _playersMutex.unlock();
    }
    if (_state == WAIT) {
        _playersMutex.lock();
        if (_players.size() == _maxPlayer || now - _lastJoin >= TIMEOUT_START_GAME) {
            this->startGame();
        } else  {
            if (now - _lastWaitMessage >= SEND_WAIT_MESSAGE_TIME) {
                _lastWaitMessage = now;
                sendToAll(StreamFactory::waitGame(TIMEOUT_START_GAME - (now - _lastJoin), false));
                now = NOW;
            }
        }
        _playersMutex.unlock();
    }

    if (_state == END) {
        if (now - _lastGameOver >= GAME_OVER_REFRESH) {
            _lastGameOver = now;
            sendToAll(StreamFactory::gameOver(0));
            now = NOW;
        }
    }
}

void Room::startGame()
{
    _state = RUN;
    _lastMapRefresh = NOW;
    _lastPlayerUpdate = NOW;
    _lastMissileUpdate = NOW;

    sendToAll(StreamFactory::waitGame(0, true, _levels.getLevel().getSong()));
    _levels.start();

    for (auto i = _players.begin(); i != _players.end(); i++)
        (**i).sendPos();
}

void Room::addMonster(IEntity::Type type, int x, int y)
{
    if (type == IEntity::Type::MISSILE || type == IEntity::Type::PLAYER)
        throw std::runtime_error("Invalid monster type");

    switch (type) {
        case IEntity::Type::LITTLE_MONSTER:
            _monsters.push_back(std::make_unique<LittleMonster>(*this, ++_monstersIds, x, y));
            break;
        case IEntity::Type::ZIGZAGER_MONSTER:
            _monsters.push_back(std::make_unique<ZigzagerMonster>(*this, ++_monstersIds, x, y));
            break;
        case IEntity::Type::FOLLOWER_MONSTER:
            _monsters.push_back(std::make_unique<FollowerMonster>(*this, ++_monstersIds, x, y));
            break;
        case IEntity::Type::BURST_MONSTER:
            _monsters.push_back(std::make_unique<BurstMonster>(*this, ++_monstersIds, x, y));
            break;
        case IEntity::Type::BOSS1:
            _monsters.push_back(std::make_unique<Boss1Monster>(*this, ++_monstersIds, x, y));
            break;
        case IEntity::Type::BOSS2:
            _monsters.push_back(std::make_unique<Boss2Monster>(*this, ++_monstersIds, x, y));
            break;
        default:
            return;
    }
    // std::cout << "Monster " << static_cast<u_int>(_monstersIds) << " spawned in room " << static_cast<int>(_id) << std::endl;
}

void Room::checkCollisionPlayer()
{
    for (auto p = _players.begin(); p != _players.end(); p++) {
        if (!(**p).exists())
            continue;
        for (auto m = _monsters.begin(); m != _monsters.end(); m++) {
            if (!(**m).exists())
                continue;
            if ((**m).collide(**p)) {
                (**p).setLife((**p).life() - (**m).getDamage());
                return;
            }
        }
    }
}

void Room::checkCollisionMonsters()
{
    for (auto p = _players.begin(); p != _players.end(); p++) {
        if (!(**p).exists())
            continue;
        for (auto m = _monsters.begin(); m != _monsters.end(); m++) {
            if (!(**m).exists())
                continue;
            if ((**p).collide(**m)) {
                (**m).setLife((**m).life() - (**p).getDamage());
                (**p).setScore((**p).score() + 10);
                return;
            }
        }
    }
}

std::pair<short, short> Room::getNearestPlayerPos(const IEntity &entity)
{
    std::pair<short, short> nearest = {0, SCREEN_HEIGHT / 2};
    double distance = std::numeric_limits<double>::max();

    for (auto p = _players.begin(); p != _players.end(); p++) {
        if (!(**p).exists())
            continue;
        double deltaX = (**p).position().first - entity.position().first;
        double deltaY = (**p).position().second - entity.position().second;
        double tmp = std::sqrt(deltaX * deltaX + deltaY * deltaY);
        if (tmp < distance) {
            distance = tmp;
            nearest = (**p).position();
        }
    }
    return nearest;
}

bool Room::isPrivate() const
{
    return _private;
}

bool Room::isMonster() const
{
    return !this->_monsters.empty();
}