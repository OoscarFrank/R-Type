#pragma once

#include <iostream>
#include <map>
#include "Game/RoomManager.hpp"

class Router {
    public:
        Router(RoomManager &rm);
        ~Router() = default;

        void route(Reader::Packet packet);

    private:
        RoomManager &_rm;
        std::map<int, void (Router::*)(Reader::Packet &packet)> _functions;

        void _movePlayer(Reader::Packet &packet);
        void _fireMissile(Reader::Packet &packet);
        void _createRoom(Reader::Packet &packet);
        void _searchRoom(Reader::Packet &packet);
        void _ping(Reader::Packet &packet);

        void _cmdNotRecieved(Reader::Packet &packet);
};
