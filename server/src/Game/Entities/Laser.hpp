#pragma once
#include "Entity.hpp"

class Laser : public AEntity
{
    public:
        Laser(Room &room, u_int id, short x, short y, bool &front);
        virtual ~Laser() = default;
        virtual void refresh();
    private:
        bool &_front;
        size_t _counter;
};