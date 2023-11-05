#pragma once
#include "Entity.hpp"

class Ray : public AEntity
{
    public:
        /**
         * @brief Construct a new Ray entity
         *
         * @param room The room the entity is in
         * @param id The id of the entity
         * @param x The x position of the entity
         * @param y The y position of the entity
         * @param front The direction the entity is facing
         * @param top The direction the entity is facing
         */
        Ray(Room &room, u_int id, short x, short y, bool &front, bool top);
        virtual ~Ray() = default;

        /**
         * @brief Refresh the entity
         *
         */
        virtual void refresh();
    private:
        bool &_front;
        size_t _counter;
        int _vx;
        int _vy;
};