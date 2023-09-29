#include "Little.hpp"
#include "../../Room.hpp"

LittleMonster::LittleMonster(Room &room, int id, short x, short y):
    ArmedEntity(room, id, x, y)
{}

LittleMonster::LittleMonster(Room &room, int id, const std::pair<short, short> &pos):
    ArmedEntity(room, id, pos)
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
        out.setDataShort(_pos.first);
        out.setDataShort(_pos.second);
        _room.sendToAll(out);
        _lastMove = now;
    }
    refreshMissiles();
}
