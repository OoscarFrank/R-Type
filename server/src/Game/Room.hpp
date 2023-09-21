#pragma once

#include <iostream>
#include "../Client.hpp"
#include <vector>
#include <thread>

class Room
{
    private:
        std::thread _thread;
        std::vector<std::shared_ptr<Client>> _clients;
        unsigned short _id;
        unsigned int _nbPlayer;
        unsigned int _maxPlayer;
        unsigned int _progress;
        unsigned int _playersIds;
        bool _private;
        void refresh();

    public:
        Room(unsigned int id, std::shared_ptr<Client> client, bool privateRoom = false);
        ~Room();
        Room(const Room &room) = delete;
        Room(Room &&room) = delete;
        Room &operator=(const Room &room) = delete;
        Room &operator=(Room &&room) = delete;
        unsigned short getId() const;
        unsigned int getNbPlayer() const;
        unsigned int getProgress() const;
        unsigned int getMaxPlayer() const;
        void addPlayer(std::shared_ptr<Client> client);
        void removePlayer(std::shared_ptr<Client> client);
};