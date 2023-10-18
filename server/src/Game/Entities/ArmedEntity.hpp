#ifndef ARMEDENTITY_HPP
#define ARMEDENTITY_HPP

#include "Entity.hpp"
#include "Missile.hpp"

class ArmedEntity: public AEntity {
    public:
        /**
         * @brief Construct a new Armed Entity object
         *
         * @param room The room the entity is in
         * @param id The id of the entity
         * @param x The x position of the entity
         * @param y The y position of the entity
         * @param w The width of the entity
         * @param h The height of the entity
         */
        ArmedEntity(Room &room, u_int id, short x, short y, short w, short h);
        /**
         * @brief Construct a new Armed Entity object
         *
         * @param room The room the entity is in
         * @param id The id of the entity
         * @param pos The position of the entity (x, y)
         * @param size The size of the entity (width, height)
         */
        ArmedEntity(Room &room, u_int id, const std::pair<short, short> &pos, const std::pair<short, short> &size);
        virtual ~ArmedEntity() = default;

        /**
         * @brief Refresh the entity (move, shoot, etc.)
         *
         */
        virtual void refresh() = 0;

    protected:
        /**
         * @brief Refresh the missiles of the entity (move, delete, etc.)
         *
         */
        virtual void refreshMissiles();
        /**
         * @brief Check if any missile of the entity collide with another entity
         *
         * @param other The other entity to check collision with
         * @return true If a missile collide with the other entity
         * @return false If no missile collide with the other entity
         */
        virtual bool missilesCollide(const IEntity &other);
        /**
         * @brief Fire a new missile
         *
         * @param type The type of the missile to fire (see Missile::Type)
         */
        virtual void fireMissile(Missile::Type type, short vx, short vy);
        std::chrono::system_clock::time_point _lastFire;

    private:
        std::vector<std::unique_ptr<Missile>> _missiles;
        std::mutex _missilesMutex;
};

#endif
