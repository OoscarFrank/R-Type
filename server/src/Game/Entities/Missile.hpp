#pragma once

#include "Entity.hpp"

class Missile: public AEntity {
    public:
        enum Type {
            ALLY = 1,
            ENEMY = 2,
        };

        Missile(Room &room, Missile::Type type, u_int id, short x, short y);
        Missile(Room &room, Missile::Type type, u_int id, const std::pair<short, short> &pos);
        virtual ~Missile();

        virtual void refresh();

    private:
        Missile::Type _type;
};
