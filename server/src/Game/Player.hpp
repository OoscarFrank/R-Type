#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "../Client.hpp"
#include "Missiles.hpp"
#include "../Utils/Instruction.hpp"
#include "../Utils/Stream.hpp"


class Room;

class Player {
    public:
        Player(Room &room, std::shared_ptr<Client> client, u_char id);
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
        const std::vector<std::unique_ptr<Missiles>> &missiles() const;
        u_char id() const;
        int score() const;
        size_t getLastMove();
        void setLastMove(size_t);

    protected:
    private:
        Room &_room;
        std::shared_ptr<Client> _client;
        std::pair<int, int> _pos;
        std::vector<std::unique_ptr<Missiles>> _missiles;
        u_char _id;
        int _score;
        size_t _lastMove;
        size_t _lastFire;

        size_t _timerTotMissilesRefresh;
        size_t _timerMissileRefresh;

};
