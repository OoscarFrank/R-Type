#pragma once

#include <iostream>
#include <map>
#include "Game/RoomManager.hpp"

class Router {
    public:
        Router(RoomManager &rm);
        ~Router() = default;

        void route(Reader::Packet packet, Levels &levels);

    private:
        RoomManager &_rm;
        std::map<int, void (Router::*)(Reader::Packet &packet, Levels &levels)> _functions;

        void _movePlayer(Reader::Packet &packet, Levels &levels);
        void _fireMissile(Reader::Packet &packet, Levels &levels);
        void _createRoom(Reader::Packet &packet, Levels &levels);
        void _searchRoom(Reader::Packet &packet, Levels &levels);
        void _ping(Reader::Packet &packet, Levels &levels);
        void _leaveRoom(Reader::Packet &packet, Levels &levels);

        void _cmdNotRecieved(Reader::Packet &packet, Levels &levels);
};
