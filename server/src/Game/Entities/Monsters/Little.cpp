#include "Little.hpp"
#include "../../Room.hpp"

LittleMonster::LittleMonster(Room &room, u_int id, short x, short y):
    ArmedEntity(room, id, x, y, LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT)
{}

LittleMonster::LittleMonster(Room &room, u_int id, const std::pair<short, short> &pos):
    ArmedEntity(room, id, pos, {LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT})
{}

LittleMonster::~LittleMonster()
{
    Stream out;
    out.setDataUChar(16);
    out.setDataUInt(_id);
    _room.sendToAll(out);
}

void LittleMonster::refresh()
{
    auto now = std::chrono::system_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= ENEMY_FIRE_TIME) {
        fireMissile(Missile::Type::ENEMY);
        _lastFire = now;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= ENEMY_MOVE_TIME) {
        move(-1, 0);
        Stream out;
        out.setDataChar(7);
        out.setDataUInt(_id);
        out.setDataShort(_box.x);
        out.setDataShort(_box.y);
        _room.sendToAll(out);
        _lastMove = now;
    }
    refreshMissiles();
}

bool LittleMonster::collide(const IEntity &other)
{
    return AEntity::collide(other) || missilesCollide(other);
}
