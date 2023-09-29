#include "ArmedEntity.hpp"
#include "../Room.hpp"

ArmedEntity::ArmedEntity(Room &room, int id, short x, short y):
    AEntity(room, id, x, y),
    _lastFire(std::chrono::system_clock::now())
{}

ArmedEntity::ArmedEntity(Room &room, int id, const std::pair<short, short> &pos):
    AEntity(room, id, pos),
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

void ArmedEntity::fireMissile(Missile::Type type)
{
    std::unique_lock<std::mutex> lock(_missilesMutex);
    _missiles.push_back(std::make_unique<Missile>(_room, type, ++_room.getMissilesIds(), _pos.first + PLAYER_WIDTH, _pos.second + PLAYER_HEIGHT / 2));
}
