#include "Room.hpp"
#include "../Utils/Scheduling.hpp"
#include <bitset>
#include <cmath>

using namespace TypesLitterals;

Room::Room(u_int id, std::shared_ptr<Client> client, Levels &levels, const std::vector<std::shared_ptr<Client>> &allClients, bool privateRoom):
    _allClients(allClients),
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
    Stream out;
    out << 27_uc << this->getId() << static_cast<u_char>(this->getNbPlayer()) << static_cast<u_char>(this->getMaxPlayer()) << 0_uc;
    for (auto i = _allClients.begin(); i != _allClients.end(); i++) {
        (*i)->send(out);
    }
}

Room::State Room::getState() const
{
    return _state;
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

    for (auto i = _players.begin(); i != _players.end(); i++)
        (**i).sendPos();
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
    return **_players.begin();
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
                _playersMutex.unlock();
                removePlayer((**i).client());
                _playersMutex.lock();
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
        handleBonus();
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
        checkCollisionBonus();

        handleForcePod();
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
    _lastBonusBoxSpawn = NOW;
    sendToAll(StreamFactory::waitGame(0, true, _levels.getLevel().getSong()));
    _levels.start();

    for (auto i = _players.begin(); i != _players.end(); i++)
        (**i).sendPos();

    Stream out;
    out << 27_uc << this->getId() << static_cast<u_char>(this->getNbPlayer()) << static_cast<u_char>(this->getMaxPlayer()) << 0_uc;
    for (auto i = _allClients.begin(); i != _allClients.end(); i++) {
        (*i)->send(out);
    }
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
        case IEntity::Type::BOSS3:
            _monsters.push_back(std::make_unique<Boss3Monster>(*this, ++_monstersIds, x, y));
            break;
        case IEntity::Type::BOSS4:
            _monsters.push_back(std::make_unique<Boss4Monster>(*this, ++_monstersIds, x, y));
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

void Room::handleBonus()
{

    if (this->_bonusBox.get() != nullptr) {
        if (!this->_bonusBox->exists()) {
            this->sendToAll(StreamFactory::bonusDestroyed(this->_bonusBox->id()));
            this->_bonusBox.release();
            this->_bonusBox = nullptr;
            return;
        }
        this->_bonusBox->refresh();
        return;
    }
    if (NOW - this->_lastBonusBoxSpawn < BONUS_SPAWN_TIME)
        return;
    this->_lastBonusBoxSpawn = NOW;
    this->_bonusBox = std::make_unique<BonusBox>(*this, ++_bonusIds);
}

void Room::checkCollisionBonus()
{
    for (auto p = _players.begin(); p != _players.end(); p++) {
        if (!(**p).exists())
            continue;
        if (this->_bonusBox != nullptr && (**p).collide((*this->_bonusBox))) {
            if ((*p)->podMissileLvl() < 3)
                (*p)->setPodMissileLvl((*p)->podMissileLvl() + 1);
            this->sendToAll(StreamFactory::bonusDestroyed(this->_bonusBox->id()));
            this->_bonusBox.release();
            this->_bonusBox = nullptr;
            return;
        }
    }
}

size_t &Room::getBombIds()
{
    return _bombIds;
}

void Room::handleForcePod()
{
    for(auto p = _players.begin(); p != _players.end(); ++p) {
        (*p)->forcePod().refresh();
        if ((*p)->forcePod().getLvl() != 1 && (*p)->score() >= POD_ONE_SCORE) {
            (*p)->forcePod().setLvl(1);
            this->sendToAll(StreamFactory::podInfo((*p)->id(), 1, 1));
        }
        for (auto m = _monsters.begin(); m != _monsters.end(); m++)
            (*p)->forcePod().bombCollide(**m);
    }
}

void Room::sendChat(std::shared_ptr<Client> client, const std::string &message)
{
    std::unique_lock<std::mutex> lock(_playersMutex);
    for (auto i = _players.begin(); i != _players.end(); i++) {
        if ((**i).client() == client) {
            _chatMessages.push_back(std::make_unique<ChatMessage>(*this, (**i).id(), message));
            return;
        }
    }
}



Room::ChatMessage::ChatMessage(Room &room, u_int playerId, const std::string &message):
    _timeStamp(std::chrono::system_clock::now()),
    _playerId(playerId),
    _message(message)
{
    std::cout << "New chat from " << playerId << ": " << message << std::endl;

    Stream out;
    out << 31_uc << playerId;
    for (auto i = message.begin(); i != message.end(); i++) {
        out << *i;
    }
    for (std::size_t i = message.size(); i < 1000; i++) {
        out << 0_c;
    }
    room.sendToAll(out);
}

Room::ChatMessage::~ChatMessage()
{}

const std::chrono::system_clock::time_point &Room::ChatMessage::getTimeStamp() const
{
    return _timeStamp;
}

u_int Room::ChatMessage::getPlayerId() const
{
    return _playerId;
}

const std::string &Room::ChatMessage::getMessage() const
{
    return _message;
}
