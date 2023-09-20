#pragma once


#include <iostream>
#include "Room.hpp"
#include <vector>

class Game
{
    private:
        std::vector<Room> _rooms;
        unsigned int _roomIds;

    public:
        Game();
        ~Game();
        void createRoom(Client &client);
};