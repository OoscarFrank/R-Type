#ifndef BOSS3MONSTER_HPP
#define BOSS3MONSTER_HPP

#include "../Entity.hpp"
#include "Monster.hpp"

class Boss3Monster: public Monster {
    public:
        /**
         * @brief Construct a new Little Monster object
         *
         * @param room The room where the entity is
         * @param id The id of the entity
         * @param x The x position of the monster
         * @param y The y position of the monster
         */
        Boss3Monster(Room &room, u_int id, short x, short y);
        /**
         * @brief Construct a new Little Monster object
         *
         * @param room The room where the entity is
         * @param id The id of the entity
         * @param pos The position of the monster (x, y)
         */
        Boss3Monster(Room &room, u_int id, const std::pair<short, short> &pos);
        virtual ~Boss3Monster() = default;

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
            int _tmpLife;
            std::chrono::system_clock::time_point _changeVelocity;
            std::chrono::system_clock::time_point _lastPosFire;
            std::chrono::system_clock::time_point _lastPos;
            short _switch;
            std::chrono::system_clock::time_point _lastPos2;
            // std::chrono::system_clock::time_point _lastDisappear;
            std::chrono::system_clock::time_point _lastAppear;
            std::pair<short, short> moveDirection;
            bool exist;
            short _vx;
            short _vy;

};

#endif
