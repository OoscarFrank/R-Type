#ifndef FOLLOWERMONSTER_HPP
#define FOLLOWERMONSTER_HPP

#include "../Entity.hpp"
#include "../ArmedEntity.hpp"

class FollowerMonster: public ArmedEntity {
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
        virtual ~FollowerMonster();

        /**
         * @brief Set the monster's life
         *
         * @param life The new life to get
         */
        virtual void setLife(int life);
        /**
         * @brief Refresh the entity
         * This method does the following:
         * - Refresh the missiles
         * - Fire missile every ENEMY_FIRE_TIME
         * - Move itself every ENEMY_MOVE_TIME and send the new position to all clients
         *
         */
        virtual void refresh();
        /**
         * @brief Check if the entity collide with another entity
         *
         * @param other The other entity to check collision with
         * @return true If the entity collide with the other entity
         * @return false If the entity doesn't collide with the other entity
         */
        virtual bool collide(const IEntity &other) override;
        virtual void removeHP();
};

#endif
