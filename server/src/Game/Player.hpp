#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "../Client.hpp"
#include "../Utils/Instruction.hpp"

class Room;
using Notifier = void (Room::*)(const std::string &);

class Player {
    public:
        Player(Room &room, Notifier sendToAll, std::shared_ptr<Client> client, u_char id);
        ~Player() = default;

        Player(const Player &player) = delete;
        Player(Player &&player) = delete;
        Player &operator=(const Player &player) = delete;
        Player &operator=(Player &&player) = delete;

        void fireMissile();
        void refreshMissiles();
        void move(int dx, int dy);

        std::shared_ptr<Client> client() const;
        const std::pair<int, int> &position() const;
        const std::vector<std::pair<int, int>> &missiles() const;
        u_char id() const;
        int score() const;

    protected:
    private:
        Room &_room;
        Notifier _sendToAll;
        std::shared_ptr<Client> _client;
        std::pair<int, int> _pos;
        std::vector<std::pair<int, int>> _missiles;
        u_char _id;
        int _score;

};
