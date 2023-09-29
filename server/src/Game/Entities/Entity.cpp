#include "Entity.hpp"
#include "../Room.hpp"

AEntity::AEntity(Room &room, int id, short x, short y):
    _room(room),
    _id(id),
    _pos(x, y),
    _lastMove(std::chrono::system_clock::now())
{}

AEntity::AEntity(Room &room, int id, const std::pair<short, short> &pos):
    _room(room),
    _id(id),
    _pos(pos),
    _lastMove(std::chrono::system_clock::now())
{}

const std::pair<short, short> &AEntity::position() const
{
    return _pos;
}

int AEntity::id() const
{
    return _id;
}

bool AEntity::isOutOfScreen() const
{
    return _pos.first < 0 || _pos.first > SCREEN_WIDTH || _pos.second < 0 || _pos.second > SCREEN_HEIGHT;
}

void AEntity::move(short dx, short dy)
{
    _pos.first += dx;
    _pos.second += dy;
}
