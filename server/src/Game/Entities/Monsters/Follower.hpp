#ifndef FOLLOWERMONSTER_HPP
#define FOLLOWERMONSTER_HPP

#include "../Entity.hpp"
#include "Monster.hpp"

class FollowerMonster: public Monster {
    public:
        /**
         * @brief Construct a new Little Monster object
         *
         * @param room The room where the entity is
         * @param id The id of the entity
         * @param x The x position of the monster
         * @param y The y position of the monster
         */
        FollowerMonster(Room &room, u_int id, short x, short y);
        /**
         * @brief Construct a new Little Monster object
         *
         * @param room The room where the entity is
         * @param id The id of the entity
         * @param pos The position of the monster (x, y)
         */
        FollowerMonster(Room &room, u_int id, const std::pair<short, short> &pos);
        virtual ~FollowerMonster() = default;

        /**
         * @brief Refresh the entity
         * This method does the following:
         * - Refresh the missiles
         * - Fire missile every ENEMY_FIRE_TIME
         * - Move itself every ENEMY_MOVE_TIME and send the new position to all clients
         *
         */
        virtual void refresh();
        virtual int getDamage();
};

#endif
