#include "Room.hpp"
#include "../Utils/Scheduling.hpp"
#include <bitset>

Room::Room(u_int id, std::shared_ptr<Client> client, bool privateRoom)
{
    _id = id;
    _playersIds = 0;
    _maxPlayer = 4;
    _progress = 0;
    _lastMapRefresh = 0;
    _state = WAIT;
    _broadcastInst = 0;

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

    setInstBroadcast(13);
    this->_broadcastStream.setDataUInt(newId);
    sendBroadcast();


    client->setInst(10);
    client->getStreamOut().setDataUInt(_id);
    client->getStreamOut().setDataUInt(newId);
    client->send();
    _players.push_back(std::make_unique<Player>(*this, client, newId, 0, 0));

    _lastJoin = NOW;
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
        if (move && nbr)
            player.sendPos();
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

void Room::sendBroadcast()
{
    Stream out;
    out += _broadcastInst;
    out += _broadcastStream;
    sendToAll(out);
    _broadcastStream.clear();
    _broadcastInst = 0;
}

void Room::refresh()
{
    while (true) {
        for (auto i = _players.begin(); i != _players.end(); i++) {
            if (!(**i).client()->isAlive()) {
                std::cout << "Player " << (**i).id() << " disconnected in room " << _id << std::endl;
                _players.erase(i);
                this->setInstBroadcast(14);
                this->_broadcastStream.setDataUInt((**i).id());
                this->sendBroadcast();
                break;
            }
        }
        update();
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Room::update()
{
    size_t now = NOW;
    if (_state == RUN) {
        if (now - _lastMapRefresh >= MAP_REFRESH_TIME) {
            _lastMapRefresh = now;
            _progress += MAP_PROGRESS_STEP;
            this->setInstBroadcast(0x01);
            this->_broadcastStream.setDataUInt(_progress);
            this->sendBroadcast();
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
            if ((**i).getDeletable()) {
                _monsters.erase(i);
                continue;
            }
            if ((**i).getExist() && (**i).isOutOfScreen()) {
                (**i).killEntity();
                this->setInstBroadcast(16);
                this->_broadcastStream.setDataUInt((**i).id());
                this->sendBroadcast();
            } else
                i++;
        }

        checkCollisionPlayer();
        checkCollisionMonsters();

        if (now - _lastMonsterSpawn >= ENEMY_SPAWN_TIME) {
            _lastMonsterSpawn = now;
            this->addMonster(IEntity::Type::LITTLE_MONSTER, SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT);
            now = NOW;
        }
        _playersMutex.unlock();
    } 
    if (_state == WAIT) {
        _playersMutex.lock();
        if (_players.size() == _maxPlayer || now - _lastJoin >= TIMEOUT_START_GAME) {
            this->startGame();
        } else  {
            if (now - _lastWaitMessage >= SEND_WAIT_MESSAGE_TIME) {
                _lastWaitMessage = now;
                this->setInstBroadcast(11);
                this->_broadcastStream.setDataInt(TIMEOUT_START_GAME - (now - _lastJoin));
                this->_broadcastStream.setDataUChar(0);
                this->sendBroadcast();
                now = NOW;
            }
        }
        _playersMutex.unlock();
    }

    if (_state == END) {
        if (now - _lastGameOver >= GAME_OVER_REFRESH) {
            _lastGameOver = now;
            this->setInstBroadcast(17);
            this->_broadcastStream.setDataChar(0);
            this->sendBroadcast();
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
    _lastMonsterSpawn = NOW;
    this->setInstBroadcast(11);
    this->_broadcastStream.setDataInt(0);
    this->_broadcastStream.setDataUChar(1);
    this->sendBroadcast();

    for (auto i = _players.begin(); i != _players.end(); i++) {
        (**i).sendPos();
    }
}

Stream &Room::getBroadcastStream()
{
    return _broadcastStream;
}


void Room::setInstBroadcast(unsigned char inst)
{
    _broadcastInst = inst;
}

void Room::addMonster(IEntity::Type type, int x, int y)
{
    if (type == IEntity::Type::MISSILE || type == IEntity::Type::PLAYER)
        throw std::runtime_error("Invalid monster type");

    switch (type) {
        case IEntity::Type::LITTLE_MONSTER:
            _monsters.push_back(std::make_unique<LittleMonster>(*this, ++_monstersIds, x, y));
            break;
        default:
            return;
    }
    // std::cout << "Monster " << static_cast<u_int>(_monstersIds) << " spawned in room " << static_cast<int>(_id) << std::endl;
}

void Room::checkCollisionPlayer()
{
    for (auto i = _players.begin(); i != _players.end(); i++) {
        for (auto j = _monsters.begin(); j != _monsters.end(); j++) {
            if ((**j).collide(**i)) {
                std::cout << "Player " << (**i).id() << " died in room " << _id << std::endl;
                // _players.erase(i);
                (**i).killEntity();
                this->setInstBroadcast(18);
                this->_broadcastStream.setDataUInt((**i).id());
                this->sendBroadcast();
                return;
            }
        }
    }
}

void Room::checkCollisionMonsters()
{
    for (auto i = _players.begin(); i != _players.end(); i++) {
        if (!(**i).getExist())
            continue;
        for (auto j = _monsters.begin(); j != _monsters.end(); j++) {
            if (!(**j).getExist())
                continue;
            if ((**i).collide(**j)) {
                (**j).killEntity();
                this->setInstBroadcast(16);
                this->_broadcastStream.setDataUInt((**j).id());
                this->sendBroadcast();
                // _monsters.erase(j);
                return;
            }
        }
    }
}
