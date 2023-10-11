#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include "Room.hpp"
#include <vector>
#include "../Reader/Reader.hpp"
#include <mutex>

class Game
{
    private:
        std::vector<std::unique_ptr<Room>> _rooms;
        u_int _roomIds;
        std::mutex _roomsMutex;

    public:
        Game();
        ~Game();
        void createRoom(Reader::Packet &packet, bool privateRoom = false);
        void searchRoom(Reader::Packet &packet);

        Room &getRoom(u_int id);
        Room &getRoom(std::shared_ptr<Client> client);
};

#endif