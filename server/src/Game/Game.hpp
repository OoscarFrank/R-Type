#pragma once


#include <iostream>
#include "Room.hpp"
#include <vector>
#include "../Reader/Reader.hpp"
#include <mutex>

class Game
{
    private:
        std::vector<std::unique_ptr<Room>> _rooms;
        unsigned int _roomIds;
        std::mutex _roomsMutex;

    public:
        Game();
        ~Game();
        void createRoom(Reader::Packet &packet, bool privateRoom = false);
        void searchRoom(Reader::Packet &packet);

        Room &getRoom(unsigned int id);
        Room &getRoom(std::shared_ptr<Client> client);
};