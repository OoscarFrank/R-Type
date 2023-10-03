#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include "../../Client.hpp"
#include "../../Utils/Scheduling.hpp"
#include "../../Utils/Instruction.hpp"
#include "../../Utils/BoundingBox.hpp"

class Room;

class IEntity {
    public:
        virtual ~IEntity() = default;

        virtual void refresh() = 0;
        virtual std::pair<short, short> position() const = 0;
        virtual u_int id() const = 0;
        virtual bool isOutOfScreen() const = 0;
        virtual bool collide(const IEntity &other) = 0;
        virtual const BoundingBox<short> &box() const = 0;
        virtual void killEntity() = 0;
        virtual bool getExist() const = 0;
        virtual bool getDeletable() const = 0;

        enum Type {
            MISSILE,
            PLAYER,
            LITTLE_MONSTER,
        };
};

class AEntity: public IEntity {
    public:
        AEntity(Room &room, u_int id, short x, short y, short w, short h);
        AEntity(Room &room, u_int id, const std::pair<short, short> &pos, const std::pair<short, short> &size);
        virtual ~AEntity() = default;

        virtual void refresh() = 0;
        virtual std::pair<short, short> position() const;
        virtual u_int id() const;
        virtual bool isOutOfScreen() const;
        virtual bool collide(const IEntity &other);
        virtual const BoundingBox<short> &box() const;
        virtual void killEntity();
        virtual bool getExist() const;
        virtual bool getDeletable() const;

    protected:
        virtual void move(short dx, short dy);

        Room &_room;
        u_int _id;
        BoundingBox<short> _box;
        bool _exist;
        bool _deletable;
        std::chrono::system_clock::time_point _lastMove;
};
