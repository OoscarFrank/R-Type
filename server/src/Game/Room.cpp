#include "Room.hpp"
#include "../Utils/Scheduling.hpp"

Room::Room(unsigned int id, std::shared_ptr<Client> client, bool privateRoom)
{
    _id = id;
    _nbPlayer = 0;
    _playersIds = 1;
    _maxPlayer = 4;
    _progress = 0;
    _lastMapRefresh = 0;
    _started = false;
    _broadcastBufferInst = 0;
    _broadcastBuffer = "";

    addPlayer(client);
    _lastJoin = NOW;
    _private = privateRoom;
    _lastWaitMessage = NOW;
    _thread = std::thread(&Room::refresh, this);

    // std::cout << "New room created with:" << std::endl;
    // for (auto i = _clients.begin(); i != _clients.end(); i++) {
    //     std::cout << i->getEndpoint().address() << std::endl;
    // }
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
    return _nbPlayer;
}

unsigned int Room::getProgress() const
{
    return _progress;
}

unsigned int Room::getMaxPlayer() const
{
    return _maxPlayer;
}

void Room::addPlayer(std::shared_ptr<Client> client)
{
    std::unique_lock<std::mutex> lock(_playersMutex);
    for (auto i = _players.begin(); i != _players.end(); i++)
        if ((**i).client() == client)
            return;

    client->setInst(0x0a);
    client->catShortOut(_id);
    client->catCharOut(_playersIds);
    _players.push_back(std::make_unique<Player>(*this, &Room::sendToAll, client, _playersIds++));
    _nbPlayer++;
    client->send();
    _lastJoin = NOW;
}

void Room::movePlayer(std::shared_ptr<Client> client, char move, char nbr)
{
    Player &player = getPlayer(client);
    for (int i = 0; i < nbr; i++) {
        if (move == PLAYER_MOVE_UP)
            player.move(-PLAYER_MOVE_OFFSET, 0);
        else if (move == PLAYER_MOVE_DOWN)
            player.move(PLAYER_MOVE_OFFSET, 0);
        else if (move == PLAYER_MOVE_LEFT)
            player.move(0, -PLAYER_MOVE_OFFSET);
        else if (move == PLAYER_MOVE_RIGHT)
            player.move(0, PLAYER_MOVE_OFFSET);
    }
}

void Room::removePlayer(std::shared_ptr<Client> client)
{
    for (auto i = _players.begin(); i != _players.end(); i++) {
        if ((**i).client() == client) {
            _players.erase(i);
            _nbPlayer--;
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

void Room::sendToAll(const std::string &message)
{
    for (auto i = _players.begin(); i != _players.end(); i++) {
        (**i).client()->send(message);
    }
}

void Room::sendBroadcast()
{
    std::string out;
    out += _broadcastBufferInst;
    out += _broadcastBuffer;
    for (auto i = _players.begin(); i != _players.end(); i++) {
        (**i).client()->send(out);
    }
    _broadcastBuffer = "";
    _broadcastBufferInst = 0;
}

void Room::refresh()
{
    while (true) {
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
            this->catIntBroadcast(_progress);
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
                this->catCharBroadcast((**i).id());
                this->catShortBroadcast((**i).position().first);
                this->catShortBroadcast((**i).position().second);
                this->sendBroadcast();
            }
            now = NOW;
        }
        _playersMutex.unlock();
    } else {
        _playersMutex.lock();
        if (_nbPlayer == _maxPlayer || now - _lastJoin >= JOIN_TIMEOUT) {
            this->startGame();
        } else  {
            while (now - _lastWaitMessage >= REFRESH_WAIT_MESSAGE) {
                _lastWaitMessage += REFRESH_WAIT_MESSAGE;
                this->setInstBroadcast(0x0b);
                this->catIntBroadcast(JOIN_TIMEOUT - (now - _lastJoin));
                this->catCharBroadcast(_started);
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
    this->catIntBroadcast(0);
    this->catCharBroadcast(true);
    this->sendBroadcast();
}

void Room::catCharBroadcast(const char &data)
{
    this->_broadcastBuffer += data;
}

void Room::catShortBroadcast(const short &data)
{
    char tmp;
    this->_broadcastBuffer += data;
    tmp = data >> 8;
    this->_broadcastBuffer += tmp;
}

void Room::catIntBroadcast(const int &data)
{
    char tmp = data;
    this->_broadcastBuffer += tmp;
    tmp = data >> 8;
    this->_broadcastBuffer += tmp;
    tmp = data >> 16;
    this->_broadcastBuffer += tmp;
    tmp = data >> 24;
    this->_broadcastBuffer += tmp;
}

void Room::setInstBroadcast(unsigned char inst)
{
    _broadcastBufferInst = inst;
}
