#pragma once
#include "../Entity.hpp"


class BonusBox : public AEntity
{
    public:
        /**
         * @brief Construct a new Bonus box entity
         *
         * @param room The room the entity is in
         * @param id The id of the entity
         */
        BonusBox(Room &room, u_int id);
        virtual ~BonusBox() = default;

        /**
         * @brief Refresh the entity
         *
         */
        virtual void refresh();
    private:
        short _vx;
        short _vy;
};