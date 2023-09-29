#include "Entity.hpp"
#include "../Room.hpp"

AEntity::AEntity(Room &room, int id, short x, short y, short w, short h):
    _room(room),
    _id(id),
    _box(x, y, w, h),
    _lastMove(std::chrono::system_clock::now())
{}

AEntity::AEntity(Room &room, int id, const std::pair<short, short> &pos, const std::pair<short, short> &size):
    _room(room),
    _id(id),
    _box(pos, size),
    _lastMove(std::chrono::system_clock::now())
{}

std::pair<short, short> AEntity::position() const
{
    return {_box.x, _box.y};
}

int AEntity::id() const
{
    return _id;
}

bool AEntity::isOutOfScreen() const
{
    return _box.x < 0 - _box.width || _box.x > SCREEN_WIDTH || _box.y < 0 - _box.height || _box.y > SCREEN_HEIGHT;
}

bool AEntity::collide(const IEntity &other)
{
    return _box.collide(other.box());
}

const BoundingBox<short> &AEntity::box() const
{
    return _box;
}

void AEntity::move(short dx, short dy)
{
    _box.x += dx;
    _box.y += dy;
}
