#include "Game.hpp"

Game::Game()
{
    _roomIds = 1;
}

Game::~Game()
{
}

void Game::createRoom(Reader::Packet &packet, asio::ip::udp::socket &socket, bool privateRoom)
{
    std::unique_ptr<Room> newRoom = std::make_unique<Room>(_roomIds, packet.getClient(), privateRoom);
    packet.getClient()->setRoomId(_roomIds);
    _roomsMutex.lock();
    _rooms.push_back(std::move(newRoom));
    packet.getClient()->setInst(0x0a);
    packet.getClient()->catShortOut(_roomIds);
    _roomIds++;
    _roomsMutex.unlock();
    packet.getClient()->catCharOut(packet.getClient()->getGamePlayerId());
    socket.send_to(asio::buffer(packet.getClient()->getOutReq()), packet.getClient()->getEndpoint());
}

void Game::searchRoom(Reader::Packet &packet, asio::ip::udp::socket &socket)
{
    _roomsMutex.lock();
    if (_rooms.size() == 0) {
        _roomsMutex.unlock();
        this->createRoom(packet, socket);
        return;
    }

    for (auto i = this->_rooms.begin(); i != this->_rooms.end(); i++) {
        if ((**i).getNbPlayer() < (**i).getMaxPlayer()) {
            packet.getClient()->setRoomId((**i).getId());
            (**i).addPlayer(packet.getClient());
            packet.getClient()->setInst(0x0a);
            packet.getClient()->catShortOut((**i).getId());
            _roomsMutex.unlock();
            packet.getClient()->catCharOut(packet.getClient()->getGamePlayerId());
            socket.send_to(asio::buffer(packet.getClient()->getOutReq()), packet.getClient()->getEndpoint());
            return;
        }
    }
    _roomsMutex.unlock();
    this->createRoom(packet, socket);
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
