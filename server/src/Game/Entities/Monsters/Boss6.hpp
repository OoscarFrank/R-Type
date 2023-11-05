#ifndef BOSS6MONSTER_HPP
#define BOSS6MONSTER_HPP

#include "../Entity.hpp"
#include "Monster.hpp"

class Boss6Monster: public Monster {
    public:
        /**
         * @brief Construct a new Little Monster object
         *
         * @param room The room where the entity is
         * @param id The id of the entity
         * @param x The x position of the monster
         * @param y The y position of the monster
         */
        Boss6Monster(Room &room, u_int id, short x, short y);
        /**
         * @brief Construct a new Little Monster object
         *
         * @param room The room where the entity is
         * @param id The id of the entity
         * @param pos The position of the monster (x, y)
         */
        Boss6Monster(Room &room, u_int id, const std::pair<short, short> &pos);
        virtual ~Boss6Monster() = default;

        /**
         * @brief Refresh the entity
         * This method does the following:
         * - Refresh the missiles
         * - Fire missile every ENEMY_LITTLE_FIRE_TIME
         * - Move itself every ENEMY_MOVE_TIME and send the new position to all clients
         *
         */
        virtual void refresh();
        virtual int getDamage();

        private:
            int _burstCount;
            std::chrono::system_clock::time_point _lastPos;
            std::chrono::system_clock::time_point _lastPos2;
            std::chrono::system_clock::time_point _lastFire2;
            std::chrono::system_clock::time_point _changeVelocity;
            std::pair<short, short> _moveDirection;
            bool exist;
            short _vx;
            short _vy;
};

#endif
