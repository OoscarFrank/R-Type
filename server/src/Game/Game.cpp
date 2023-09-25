#include "Game.hpp"

Game::Game()
{
    _roomIds = 1;
}

Game::~Game()
{
}

void Game:: createRoom(Reader::Packet &packet, bool privateRoom)
{
    std::unique_ptr<Room> newRoom = std::make_unique<Room>(_roomIds++, packet.getClient(), privateRoom);
    _roomsMutex.lock();
    _rooms.push_back(std::move(newRoom));
    _roomsMutex.unlock();
}

void Game::searchRoom(Reader::Packet &packet)
{
    _roomsMutex.lock();
    if (_rooms.size() == 0) {
        _roomsMutex.unlock();
        this->createRoom(packet);
        return;
    }

    for (auto i = this->_rooms.begin(); i != this->_rooms.end(); i++) {
        if ((**i).getNbPlayer() < (**i).getMaxPlayer()) {
            (**i).addPlayer(packet.getClient());
            _roomsMutex.unlock();
            return;
        }
    }
    _roomsMutex.unlock();
    this->createRoom(packet);
}

Room &Game::getRoom(unsigned int id)
{
    std::unique_lock<std::mutex> lock(_roomsMutex);

    for (auto i = this->_rooms.begin(); i != this->_rooms.end(); i++) {
        if ((**i).getId() == id) {
            return **i;
        }
    }
    throw std::runtime_error("Room not found");
}

Room &Game::getRoom(std::shared_ptr<Client> client)
{
    std::unique_lock<std::mutex> lock(_roomsMutex);

    for (auto i = this->_rooms.begin(); i != this->_rooms.end(); i++) {
        if ((**i).isClientInRoom(client)) {
            return **i;
        }
    }
    throw std::runtime_error("Room not found");
}
