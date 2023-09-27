#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "../Missiles.hpp"
#include "../../Utils/Stream.hpp"

class Room;

class IMonster {
    public:
        virtual ~IMonster() = default;
        virtual void refresh() = 0;
        virtual const std::pair<int, int> &position() const = 0;
        virtual const std::vector<std::unique_ptr<Missiles>> &missiles() const = 0;
        virtual int id() const = 0;

        enum Type {
            LITTLE,
            // MEDIUM,
            // BIG,
            // BOSS
        };

        static std::unique_ptr<IMonster> create(Type type, Room &room, int id, int x, int y);
};

class AMonster: public IMonster {
    public:
        AMonster(Room &room, int id, int x, int y);
        virtual ~AMonster() = default;

        virtual void refresh() = 0;

        virtual const std::pair<int, int> &position() const;
        virtual const std::vector<std::unique_ptr<Missiles>> &missiles() const;
        virtual int id() const;

    protected:
        virtual void fireMissile();
        virtual void move(int dx, int dy);

        Room &_room;
        std::pair<int, int> _pos;
        std::vector<std::unique_ptr<Missiles>> _missiles;
        int _id;
};
