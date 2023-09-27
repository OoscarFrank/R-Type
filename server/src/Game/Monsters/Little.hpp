#pragma once

#include <iostream>
#include <chrono>
#include "Monster.hpp"

class Little: public AMonster {
    public:
        Little(Room &room, size_t id, int x, int y);
        ~Little() = default;

        void refresh();

    private:
        std::chrono::system_clock::time_point _lastFire;
        std::chrono::system_clock::time_point _lastMove;
};
