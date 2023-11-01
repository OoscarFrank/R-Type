#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "Entity.hpp"

class Missile: public AEntity {
    public:
        enum Type {
            PLAYER_ONE = 1,
            LITTLE_MONSTER,
            ORANGE_MISSILE,
            PURPLE_MISSILE,
            GREEN_MISSILE,
            PLAYER_TWO,
            PLAYER_THREE,
        };

        /**
         * @brief Construct a new Missile object
         *
         * @param room The room where the missile is
         * @param type The type of the missile
         * @param id The id of the missile
         * @param x The x position of the missile
         * @param y The y position of the missile
         */
        Missile(Room &room, Missile::Type type, u_int id, short x, short y, short vx, short vy);
        /**
         * @brief Construct a new Missile object
         *
         * @param room The room where the missile is
         * @param type The type of the missile
         * @param id The id of the missile
         * @param pos The position of the missile (x, y)
         */
        Missile(Room &room, Missile::Type type, u_int id, const std::pair<short, short> &pos, const std::pair<short, short> &velocity);
        virtual ~Missile();

        /**
         * @brief Refresh the missile (move and send the new position to the clients)
         *
         */
        virtual void refresh();

    private:
        Missile::Type _type;
        short _vx;
        short _vy;
};

#endif
