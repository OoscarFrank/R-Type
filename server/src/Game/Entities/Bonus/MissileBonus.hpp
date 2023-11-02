#pragma once
#include "../Entity.hpp"


class MissileBonus : public AEntity
{
    public:
        MissileBonus(Room &room, u_int id);
        virtual ~MissileBonus() = default;

        virtual void refresh();
    private:
        short _vx;
        short _vy;
};