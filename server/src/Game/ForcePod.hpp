#pragma once
#include <iostream>
#include <vector>
#include "Entities/Bomb.hpp"
#include <memory>
#include <mutex>

class Room;
class Player;

class ForcePod {
    public:
        ForcePod(Room &room, Player &player);

        void refresh();
        void toggleFront();
        void shootBomb();
        void bombCollide(IEntity &other);
        void setLvl(u_char lvl);
        u_char getLvl() const;
        std::mutex _mutex;
    private:
        bool _isFront;
        std::chrono::system_clock::time_point _lastBomb;
        std::chrono::system_clock::time_point _lastRay;
        std::chrono::system_clock::time_point _lastLaser;
        u_char lvl = 0;
        std::vector<std::unique_ptr<Bomb>> _bombs;
        Room &_room;
        Player &_player;
};