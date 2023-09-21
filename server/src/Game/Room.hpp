#pragma once

#include <iostream>
#include "../Client.hpp"
#include <vector>
#include <thread>

class Room
{
    private:
        asio::ip::udp::socket &_socket;
        std::thread _thread;
        std::vector<Client> _clients;
        unsigned short _id;
        unsigned int _nbPlayer;
        unsigned int _maxPlayer;
        unsigned int _progress;
        unsigned int _playersIds;
        bool _private;
        void refresh();

    public:
        Room(asio::ip::udp::socket &socket, unsigned int id, Client &client, bool privateRoom = false);
        ~Room();
        Room(const Room &room) = delete;
        Room(Room &&room) = delete;
        Room &operator=(const Room &room) = delete;
        Room &operator=(Room &&room) = delete;
        unsigned short getId() const;
        unsigned int getNbPlayer() const;
        unsigned int getProgress() const;
        unsigned int getMaxPlayer() const;
        void addPlayer(Client &client);
        void removePlayer(Client &client);
};