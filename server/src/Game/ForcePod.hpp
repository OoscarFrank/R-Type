#pragma once
#include <iostream>
#include <vector>
#include "Entities/Bomb.hpp"
#include "Entities/Laser.hpp"
#include <memory>
#include <mutex>
#include "Entities/Ray.hpp"
#include "Entities/Laser.hpp"
#include "Entities/Bomb.hpp"

class Room;
class Player;

class ForcePod {
    public:
        ForcePod(Room &room, Player &player);

        void refresh();
        void toggleFront();
        void shootBomb();
        void shootLaser();
        void shootRay();
        void bombCollide(IEntity &other);
        void laserCollide(IEntity &other);
        void rayCollide(IEntity &other);
        void setLvl(u_char lvl);
        u_char getLvl() const;
        std::mutex _mutex;
        std::chrono::system_clock::time_point _lastLaserHit;
        std::chrono::system_clock::time_point _lastRayHit;
    private:
        bool _isFront;
        std::chrono::system_clock::time_point _lastBomb;
        std::chrono::system_clock::time_point _lastRay;
        std::chrono::system_clock::time_point _lastLaser;


        u_char lvl = 0;
        std::vector<std::unique_ptr<Bomb>> _bombs;
        std::vector<std::unique_ptr<Laser>> _lasers;
        std::vector<std::unique_ptr<Ray>> _rays;
        Room &_room;
        Player &_player;
};