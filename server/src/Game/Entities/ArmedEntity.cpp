#include "ArmedEntity.hpp"
#include "../Room.hpp"

ArmedEntity::ArmedEntity(Room &room, u_int id, short x, short y, short w, short h):
    AEntity(room, id, x, y, w, h),
    _lastFire(std::chrono::system_clock::now())
{}

ArmedEntity::ArmedEntity(Room &room, u_int id, const std::pair<short, short> &pos, const std::pair<short, short> &size):
    AEntity(room, id, pos, size),
    _lastFire(std::chrono::system_clock::now())
{}

void ArmedEntity::refreshMissiles()
{
    std::unique_lock<std::mutex> lock(_missilesMutex);

    for (auto i = _missiles.begin(); i != _missiles.end();) {
        (**i).refresh();
        if ((**i).isOutOfScreen())
            i = _missiles.erase(i);
        else
            i++;
    }
}

bool ArmedEntity::missilesCollide(const IEntity &other)
{
    std::unique_lock<std::mutex> lock(_missilesMutex);

    for (auto &missile: _missiles)
        if (missile->collide(other))
            return true;
    return false;
}

void ArmedEntity::fireMissile(Missile::Type type)
{
    std::unique_lock<std::mutex> lock(_missilesMutex);
    _missiles.push_back(std::make_unique<Missile>(_room, type, ++_room.getMissilesIds(), _box.x + PLAYER_WIDTH, _box.y + PLAYER_HEIGHT / 2));
}
