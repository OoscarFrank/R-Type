#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include "../../Client.hpp"
#include "../../Utils/Scheduling.hpp"
#include "../../Utils/Instruction.hpp"

class Room;

class IEntity {
    public:
        virtual ~IEntity() = default;

        virtual void refresh() = 0;
        virtual const std::pair<short, short> &position() const = 0;
        virtual int id() const = 0;
        virtual bool isOutOfScreen() const = 0;

        enum Type {
            MISSILE,
            PLAYER,
            LITTLE_MONSTER,
        };
};

class AEntity: public IEntity {
    public:
        AEntity(Room &room, int id, short x, short y);
        AEntity(Room &room, int id, const std::pair<short, short> &pos);
        virtual ~AEntity() = default;

        virtual void refresh() = 0;
        virtual const std::pair<short, short> &position() const;
        virtual int id() const;
        virtual bool isOutOfScreen() const;

    protected:
        virtual void move(short dx, short dy);

        Room &_room;
        int _id;
        std::pair<short, short> _pos;
        std::chrono::system_clock::time_point _lastMove;
};




