#pragma once
#include "Entity.hpp"

class Ray : public AEntity
{
    public:
        Ray(Room &room, u_int id, short x, short y, bool &front, bool top);
        virtual ~Ray() = default;
        virtual void refresh();
    private:
        bool &_front;
        size_t _counter;
        int _vx;
        int _vy;
};