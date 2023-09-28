#include "Monster.hpp"
#include "../Room.hpp"

#include "Little.hpp"

AMonster::AMonster(Room &room, int id, int x, int y):
    _room(room),
    _pos(x, y),
    _id(id)
{}

const std::pair<int, int> &AMonster::position() const
{
    return _pos;
}

const std::vector<std::unique_ptr<Missiles>> &AMonster::missiles() const
{
    return _missiles;
}

int AMonster::id() const
{
    return _id;
}

void AMonster::fireMissile()
{
    ++_room.getMissilesIds();
    _missiles.push_back(std::make_unique<Missiles>(_pos.first, _pos.second, _room.getMissilesIds(), Missiles::ENEMY, _room));
}

void AMonster::move(int dx, int dy)
{
    _pos.first += dx;
    _pos.second += dy;
    Stream out;
    out.setDataChar(7);
    out.setDataChar(_id);
    out.setDataShort(_pos.first);
    out.setDataShort(_pos.second);
    _room.sendToAll(out);
}

std::unique_ptr<IMonster> IMonster::create(Type type, Room &room, int id, int x, int y)
{
    switch (type) {
        case LITTLE:
            return std::make_unique<Little>(room, id, x, y);
        // case MEDIUM:
        //     return std::make_unique<Medium>(room, id, x, y);
        // case BIG:
        //     return std::make_unique<Big>(room, id, x, y);
        // case BOSS:
        //     return std::make_unique<Boss>(room, id, x, y);
        default:
            return nullptr;
    }
}
