#pragma once
#include "Entity.hpp"

class Bomb : public AEntity
{
    public:
        /**
         * @brief Construct a new Bomb entity
         *
         * @param room The room the entity is in
         * @param id The id of the entity
         * @param x The x position of the entity
         * @param y The y position of the entity
         * @param front The direction the entity is facing
         */
        Bomb(Room &room, u_int id, short x, short y, bool &front);
        virtual ~Bomb() = default;

        /**
         * @brief Refresh the entity
         *
         */
        virtual void refresh();
    private:
        short _yCounter;
        short _vx;
        bool &_front;
};