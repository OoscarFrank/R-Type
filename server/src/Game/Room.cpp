#include "Room.hpp"
#include "../Utils/Scheduling.hpp"
#include <bitset>

Room::Room(unsigned int id, std::shared_ptr<Client> client, bool privateRoom)
{
    _id = id;
    _playersIds = 1;
    _maxPlayer = 4;
    _progress = 0;
    _lastMapRefresh = 0;
    _started = false;
    _broadcastInst = 0;

    _missilesIds = 0;

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

unsigned short Room::getId() const
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

size_t &Room::getMissilesIds()
{
    return _missilesIds;
}

void Room::addPlayer(std::shared_ptr<Client> client)
{
    std::unique_lock<std::mutex> lock(_playersMutex);
    for (auto i = _players.begin(); i != _players.end(); i++)
        if ((**i).client() == client)
            return;

    u_char newId = _playersIds++;

    setInstBroadcast(13);
    this->_broadcastStream.setDataChar(newId);
    sendBroadcast();


    client->setInst(0x0a);
    client->getStreamOut().setDataShort(_id);
    client->getStreamOut().setDataChar(newId);
    client->send();
    _players.push_back(std::make_unique<Player>(*this, client, newId));

    _lastJoin = NOW;
}

void Room::movePlayer(std::shared_ptr<Client> client, char move, char nbr)
{
    Player &player = getPlayer(client);
    if (NOW - player.getLastMove() >= MOVE_TIME) {
        for (int i = 0; i < nbr; i++) {
            if (move & PLAYER_MOVE_UP && player.position().second > 0)
                player.move(0, -PLAYER_MOVE_OFFSET);
            if (move & PLAYER_MOVE_DOWN && player.position().second < SCREEN_HEIGHT - PLAYER_HEIGHT)
                player.move(0, PLAYER_MOVE_OFFSET);
            if (move & PLAYER_MOVE_LEFT && player.position().first > 0)
                player.move(-PLAYER_MOVE_OFFSET, 0);
            if (move & PLAYER_MOVE_RIGHT && player.position().first < SCREEN_WIDTH - PLAYER_WIDTH)
                player.move(PLAYER_MOVE_OFFSET, 0);
        }
        player.setLastMove(NOW);
    }
}

void Room::removePlayer(std::shared_ptr<Client> client)
{
    for (auto i = _players.begin(); i != _players.end(); i++) {
        if ((**i).client() == client) {
            setInstBroadcast(14);
            this->_broadcastStream.setDataChar((**i).id());
            _players.erase(i);
            sendBroadcast();
            break;
        }
    }
}

bool Room::isClientInRoom(std::shared_ptr<Client> client)
{
    for (auto i = _players.begin(); i != _players.end(); i++) {
        if ((**i).client() == client) {
            return true;
        }
    }
    return false;
}

Player &Room::getPlayer(std::shared_ptr<Client> client)
{
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
                removePlayer((**i).client());
                std::cout << "Player disconnected in room " << _id << std::endl;
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
    if (_started) {
        while (now - _lastMapRefresh >= REFRESH_MAP) {
            _lastMapRefresh += REFRESH_MAP;
            _progress += MAP_PROGRESS_STEP;
            this->setInstBroadcast(0x01);
            this->_broadcastStream.setDataInt(_progress);
            this->sendBroadcast();
            now = NOW;
        }
        _playersMutex.lock();
        while (now - _lastMissileUpdate >= REFRESH_MISSILES) {
            _lastMissileUpdate += REFRESH_MISSILES;
            for (auto i = _players.begin(); i != _players.end(); i++) {
                (**i).refreshMissiles();
            }
            now = NOW;
        }
        while (now - _lastPlayerUpdate >= REFRESH_PLAYERS) {
            _lastPlayerUpdate += REFRESH_PLAYERS;
            for (auto i = _players.begin(); i != _players.end(); i++) {
                (**i).position();
                this->setInstBroadcast(0x03);
                this->_broadcastStream.setDataChar((**i).id());
                this->_broadcastStream.setDataShort((**i).position().first);
                this->_broadcastStream.setDataShort((**i).position().second);
                this->sendBroadcast();
            }
            now = NOW;
        }
        _playersMutex.unlock();
    } else {
        _playersMutex.lock();
        if (_players.size() == _maxPlayer || now - _lastJoin >= JOIN_TIMEOUT) {
            this->startGame();
        } else  {
            while (now - _lastWaitMessage >= REFRESH_WAIT_MESSAGE) {
                _lastWaitMessage += REFRESH_WAIT_MESSAGE;
                this->setInstBroadcast(0x0b);
                this->_broadcastStream.setDataInt(JOIN_TIMEOUT - (now - _lastJoin));
                this->_broadcastStream.setDataChar(_started);
                this->sendBroadcast();
                now = NOW;
            }
        }
        _playersMutex.unlock();
    }
}

void Room::startGame()
{
    _started = true;
    _lastMapRefresh = NOW;
    _lastPlayerUpdate = NOW;
    _lastMissileUpdate = NOW;
    this->setInstBroadcast(0x0b);
    this->_broadcastStream.setDataInt(0);
    this->_broadcastStream.setDataChar(1);
    this->sendBroadcast();
}

Stream &Room::getBroadcastStream()
{
    return _broadcastStream;
}


void Room::setInstBroadcast(unsigned char inst)
{
    _broadcastInst = inst;
}
