#pragma once

#include <iostream>
#include "../Client.hpp"
#include <vector>

class Room
{
    private:
        std::vector<Client> _clients;
        unsigned int _id;
        unsigned int _nbPlayer;
        unsigned int _progress;
        bool _private;
        

    public:
        Room(unsigned int id, Client &client);
        ~Room();
        unsigned int getId() const;
        unsigned int getNbPlayer() const;
        unsigned int getProgress() const;
        void addPlayer(Client &client);
        void removePlayer(Client &client);
};