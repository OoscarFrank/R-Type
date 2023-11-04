#pragma once

#include <iostream>
#include <map>
#include "Game/RoomManager.hpp"

class Router {
    public:
        Router(RoomManager &rm, const std::vector<std::shared_ptr<Client>> &allClients);
        ~Router() = default;

        void route(Reader::Packet packet, Levels &levels);

    private:
        RoomManager &_rm;
        const std::vector<std::shared_ptr<Client>> &_allClients;
        std::map<int, void (Router::*)(Reader::Packet &packet, Levels &levels)> _functions;

        void _movePlayer(Reader::Packet &packet, Levels &levels);
        void _fireMissile(Reader::Packet &packet, Levels &levels);
        void _createRoom(Reader::Packet &packet, Levels &levels);
        void _searchRoom(Reader::Packet &packet, Levels &levels);
        void _ping(Reader::Packet &packet, Levels &levels);
        void _leaveRoom(Reader::Packet &packet, Levels &levels);
        void _joinRoom(Reader::Packet &packet, Levels &levels);
        void _listRooms(Reader::Packet &packet, Levels &levels);
        void _fireBomb(Reader::Packet &packet, Levels &levels);
        void _sendChat(Reader::Packet &packet, Levels &levels);
        void _fireLaser(Reader::Packet &packet, Levels &levels);
        void _fireRay(Reader::Packet &packet, Levels &levels);

        void _cmdNotRecieved(Reader::Packet &packet, Levels &levels);
};
