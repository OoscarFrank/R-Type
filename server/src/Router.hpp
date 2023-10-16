#pragma once

#include <iostream>
#include <map>
#include "Game/Game.hpp"

class Router {
    public:
        Router(Game &game);
        ~Router() = default;

        void route(Reader::Packet packet);

    private:
        Game &_game;
        std::map<int, void (Router::*)(Reader::Packet &packet)> _functions;

        void _movePlayer(Reader::Packet &packet);
        void _fireMissile(Reader::Packet &packet);
        void _createRoom(Reader::Packet &packet);
        void _searchRoom(Reader::Packet &packet);
        void _ping(Reader::Packet &packet);
};
