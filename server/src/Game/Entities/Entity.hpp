#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include "../../Client.hpp"
#include "../../Utils/Scheduling.hpp"
#include "../../Utils/Instruction.hpp"
#include "../../Utils/BoundingBox.hpp"

class Room;

/**
 * @brief Interface for all entities
 *
 */
class IEntity {
    public:
        virtual ~IEntity() = default;

        /**
         * @brief Refresh the entity (move, shoot, etc.)
         *
         */
        virtual void refresh() = 0;
        /**
         * @brief Get the position of the entity
         *
         * @return std::pair<short, short>
         */
        virtual std::pair<short, short> position() const = 0;
        /**
         * @brief Get the id of the entity
         *
         * @return u_int
         */
        virtual u_int id() const = 0;
        /**
         * @brief Check if the entity is out of the screen
         *
         * @return true
         * @return false
         */
        virtual bool isOutOfScreen() const = 0;
        /**
         * @brief Check if the entity collide with another entity
         *
         * @param other The other entity to check collision with
         * @return true
         * @return false
         */
        virtual bool collide(const IEntity &other) = 0;
        /**
         * @brief Get the bounding box of the entity (used to check collision)
         *
         * @return const BoundingBox<short>&
         */
        virtual const BoundingBox<short> &box() const = 0;
        /**
         * @brief Get the entities life
         *
         * @param life The new life to set
         */
        virtual int life() const = 0;
        /**
         * @brief Set the entities life
         *
         * @param life The new life to get
         */
        virtual void setLife(int life) = 0;
        /**
         * @brief Kill the entity
         *
         */
        virtual void kill() = 0;
        /**
         * @brief Check if the entity exist (alive)
         *
         * @return true
         * @return false
         */
        virtual bool exists() const = 0;
        /**
         * @brief Check if the entity is deletable (dead)
         *
         * @return true
         * @return false
         */
        virtual bool isDeletable() const = 0;

        enum Type {
            MISSILE,
            PLAYER,
            LITTLE_MONSTER,
            ZIGZAGER_MONSTER,
            FOLLOWER_MONSTER,
        };
};

class AEntity: public IEntity {
    public:
        /**
         * @brief Construct a new AEntity
         *
         * @param room The room where the entity is
         * @param id The id of the entity
         * @param x The x position of the entity
         * @param y The y position of the entity
         * @param w The width of the entity
         * @param h The height of the entity
         */
        AEntity(Room &room, u_int id, short x, short y, short w, short h);
        /**
         * @brief Construct a new AEntity
         *
         * @param room The room where the entity is
         * @param id The id of the entity
         * @param pos The position of the entity (x, y)
         * @param size The size of the entity (width, height)
         */
        AEntity(Room &room, u_int id, const std::pair<short, short> &pos, const std::pair<short, short> &size);
        virtual ~AEntity() = default;

        /**
         * @brief Refresh the entity (move, shoot, etc.)
         * This method is to be implemented in the child class
         *
         */
        virtual void refresh() = 0;
        /**
         * @brief Get the position of the entity
         *
         * @return std::pair<short, short>
         */
        virtual std::pair<short, short> position() const;
        /**
         * @brief Get the id of the entity
         *
         * @return u_int
         */
        virtual u_int id() const;
        /**
         * @brief Check if the entity is out of the screen
         *
         * @return true
         * @return false
         */
        virtual bool isOutOfScreen() const;
        /**
         * @brief Check if the entity collide with another entity
         *
         * @param other The other entity to check collision with
         * @return true
         * @return false
         */
        virtual bool collide(const IEntity &other);
        /**
         * @brief Get the bounding box of the entity (used to check collision)
         *
         * @return const BoundingBox<short>&
         */
        virtual const BoundingBox<short> &box() const;
        /**
         * @brief Get the entities life
         *
         * @param life The new life to set
         */
        virtual int life() const;
        /**
         * @brief Set the entities life
         *
         * @param life The new life to get
         */
        virtual void setLife(int life);
        /**
         * @brief Kill the entity
         *
         */
        virtual void kill();
        /**
         * @brief Check if the entity exist (alive)
         *
         * @return true
         * @return false
         */
        virtual bool exists() const;
        /**
         * @brief Check if the entity is deletable (dead)
         *
         * @return true
         * @return false
         */
        virtual bool isDeletable() const;

    protected:
        /**
         * @brief Move the entity
         *
         * @param dx The x offset
         * @param dy The y offset
         */
        virtual void move(short dx, short dy);

        int _life;
        Room &_room;
        u_int _id;
        BoundingBox<short> _box;
        bool _exist;
        bool _deletable;
        std::chrono::system_clock::time_point _lastMove;
};

#endif
