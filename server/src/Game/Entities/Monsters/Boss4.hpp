#ifndef BOSS4MONSTER_HPP
#define BOSS4MONSTER_HPP

#include "../Entity.hpp"
#include "Monster.hpp"

class Boss4Monster: public Monster {
    public:
        /**
         * @brief Construct a new Little Monster object
         *
         * @param room The room where the entity is
         * @param id The id of the entity
         * @param x The x position of the monster
         * @param y The y position of the monster
         */
        Boss4Monster(Room &room, u_int id, short x, short y);
        /**
         * @brief Construct a new Little Monster object
         *
         * @param room The room where the entity is
         * @param id The id of the entity
         * @param pos The position of the monster (x, y)
         */
        Boss4Monster(Room &room, u_int id, const std::pair<short, short> &pos);
        virtual ~Boss4Monster() = default;

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
            void _fireMissilesAllDirections();
            int _burstCount;
            u_char _nbrBurst;
            std::chrono::system_clock::time_point _lastPos;
            std::chrono::system_clock::time_point _lastPos2;
            std::chrono::system_clock::time_point _teleport;
            std::chrono::system_clock::time_point _burst;
            std::chrono::system_clock::time_point _burstSpam;
            std::pair<short, short> moveDirection;
            int count;

};

#endif
