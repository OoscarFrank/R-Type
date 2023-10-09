#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "Entity.hpp"

class Missile: public AEntity {
    public:
        enum Type {
            PLAYER = 1,
            LITTLE_MONSTER = 2,
        };

        Missile(Room &room, Missile::Type type, u_int id, short x, short y);
        Missile(Room &room, Missile::Type type, u_int id, const std::pair<short, short> &pos);
        virtual ~Missile();

        virtual void refresh();

    private:
        Missile::Type _type;
};

#endif
