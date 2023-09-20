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
    Room room(_roomIds, packet.getClient(), privateRoom );
    packet.getClient().setRoomId(_roomIds);
    _rooms.push_back(room);
    packet.getClient().setInst(0x0a);
    packet.getClient().catShortOut(_roomIds);
    packet.getClient().catCharOut(packet.getClient().getGamePlayerId());
    socket.send_to(asio::buffer(packet.getClient().getOutReq()), packet.getClient().getEndpoint());
    _roomIds++;
}

void Game::searchRoom(Reader::Packet &packet, asio::ip::udp::socket &socket)
{
    if (_rooms.size() == 0) {
        this->createRoom(packet, socket);
        return;
    }

    for (auto i = this->_rooms.begin(); i != this->_rooms.end(); i++) {
        if (i->getNbPlayer() < i->getMaxPlayer()) {
            packet.getClient().setRoomId(i->getId());
            i->addPlayer(packet.getClient());
            packet.getClient().setInst(0x0a);
            packet.getClient().catShortOut(i->getId());
            packet.getClient().catCharOut(packet.getClient().getGamePlayerId());
            socket.send_to(asio::buffer(packet.getClient().getOutReq()), packet.getClient().getEndpoint());
            return;
        }
    }
}