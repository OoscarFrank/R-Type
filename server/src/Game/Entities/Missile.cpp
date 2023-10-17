#include "Missile.hpp"
#include "../Room.hpp"

Missile::Missile(Room &room, Missile::Type type, u_int id, short x, short y):
    AEntity(room, id, x, y, MISSILE_WIDTH, MISSILE_HEIGHT),
    _type(type)
{}

Missile::Missile(Room &room, Missile::Type type, u_int id, const std::pair<short, short> &pos):
    AEntity(room, id, pos, {MISSILE_WIDTH, MISSILE_HEIGHT}),
    _type(type)
{}

Missile::~Missile()
{
    _room.sendToAll(StreamFactory::missileDestroyed(_id, _type, _box.x, _box.y));
}

void Missile::refresh()
{
    auto now = std::chrono::system_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= MISSILES_REFRESH_TIME) {
        move(_type == Missile::Type::PLAYER ? PLAYER_MISSILE_PROGRESS_STEP : -ENEMY_MISSILE_PROGRESS_STEP, 0);
        _room.sendToAll(StreamFactory::missilePos(_id, _type, _box.x, _box.y));
        _lastMove = now;
    }
}
