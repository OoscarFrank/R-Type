#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "../Client.hpp"

class Room;
using Notifier = void (Room::*)(const std::string &);

class Player {
    public:
        Player(Room &room, Notifier sendToAll, std::shared_ptr<Client> client, u_char &id, int missileRange);
        ~Player() = default;

        Player(const Player &player) = delete;
        Player(Player &&player) = delete;
        Player &operator=(const Player &player) = delete;
        Player &operator=(Player &&player) = delete;

        void fireMissile();
        void refreshMissiles();
        void move(float dx, float dy);

        std::shared_ptr<Client> client() const;
        const std::pair<float, float> &position() const;
        u_char id() const;
        int missileRange() const;
        int score() const;

    protected:
    private:
        Room &_room;
        Notifier _sendToAll;
        std::shared_ptr<Client> _client;
        std::pair<float, float> _pos;
        std::vector<std::pair<float, float>> _missiles;
        u_char &_id;
        int _missileRange;
        int _score;

};
