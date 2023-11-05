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

    if (!_exist && _missiles.size() == 0) {
        _deletable = true;
        return;
    }

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

    for (auto i = _missiles.begin(); i != _missiles.end(); ++i) {
        if ((**i).collide(other)) {
            _missiles.erase(i);
            return true;
        }
    }
    return false;
}

void ArmedEntity::fireMissile(Missile::Type type, short vx, short vy)
{
    std::unique_lock<std::mutex> lock(_missilesMutex);
    if (type == Missile::Type::PLAYER_ONE || type == Missile::Type::PLAYER_TWO || type == Missile::Type::LITTLE_MONSTER || type == Missile::Type::PURPLE_MISSILE || type == Missile::Type::FIRE_BALL)
        _missiles.push_back(std::make_unique<Missile>(_room, type, ++_room.getMissilesIds(), _box.x + _box.width / 2, _box.y + _box.height / 2, vx, vy));
    else if (type == Missile::Type::PLAYER_THREE) {
        _missiles.push_back(std::make_unique<Missile>(_room, type, ++_room.getMissilesIds(), _box.x + _box.width / 2, (_box.y + _box.height / 2) - 20, vx, vy));
        _missiles.push_back(std::make_unique<Missile>(_room, type, ++_room.getMissilesIds(), _box.x + _box.width / 2, _box.y + _box.height / 2 + 20, vx, vy));
    }
}

void ArmedEntity::fireMissile(Missile::Type type, short vx, short vy, short posX, short posY)
{
    std::unique_lock<std::mutex> lock(_missilesMutex);
    _missiles.push_back(std::make_unique<Missile>(_room, type, ++_room.getMissilesIds(), posX, posY, vx, vy));
}
