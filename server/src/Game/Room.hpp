#pragma once

#include <iostream>
#include "../Client.hpp"
#include <vector>

class Room
{
    private:
        std::vector<Client> _clients;
        unsigned short _id;
        unsigned int _nbPlayer;
        unsigned int _maxPlayer;
        unsigned int _progress;
        unsigned int _playersIds;
        bool _private;
        

    public:
        Room(unsigned int id, Client &client, bool privateRoom = false);
        ~Room();
        unsigned short getId() const;
        unsigned int getNbPlayer() const;
        unsigned int getProgress() const;
        unsigned int getMaxPlayer() const;
        void addPlayer(Client &client);
        void removePlayer(Client &client);
};