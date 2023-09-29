#include "Little.hpp"
#include "../../Room.hpp"

LittleMonster::LittleMonster(Room &room, int id, short x, short y):
    ArmedEntity(room, id, x, y, LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT)
{}

LittleMonster::LittleMonster(Room &room, int id, const std::pair<short, short> &pos):
    ArmedEntity(room, id, pos, {LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT})
{}

void LittleMonster::refresh()
{
    auto now = std::chrono::system_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= 1000) { //TODO mettre un define pour le temps de tir
        fireMissile(Missile::Type::ENEMY);
        _lastFire = now;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= 2) { //TODO mettre un define pour le temps de déplacement
        move(-1, 0);
        Stream out;
        out.setDataChar(7);
        out.setDataChar(static_cast<u_char>(_id));
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
