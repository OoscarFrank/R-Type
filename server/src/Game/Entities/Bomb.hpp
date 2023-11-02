#pragma once
#include "Entity.hpp"

class Bomb : public AEntity
{
    public:
        Bomb(Room &room, u_int id, short x, short y, bool &front);
        virtual ~Bomb() = default;

        virtual void refresh();
    private:
        short _yCounter;
        short _vx;
        bool &_front;
};