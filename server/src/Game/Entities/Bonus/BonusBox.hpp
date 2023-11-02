#pragma once
#include "../Entity.hpp"


class BonusBox : public AEntity
{
    public:
        BonusBox(Room &room, u_int id);
        virtual ~BonusBox() = default;

        virtual void refresh();
    private:
        short _vx;
        short _vy;
};