#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include "Player.hpp"
#include "../Client.hpp"

class Room
{
    private:
        std::thread _thread;
        std::vector<std::unique_ptr<Player>> _players;
        unsigned short _id;
        unsigned int _nbPlayer;
        unsigned int _maxPlayer;
        unsigned int _progress;
        u_char _playersIds;
        bool _private;
        void refresh();
        void updateEntities();
        void sendUpdateClients();

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
        bool isClientInRoom(std::shared_ptr<Client> client);
        Player &getPlayer(std::shared_ptr<Client> client);
        void sendToAll(const std::string &message);
};