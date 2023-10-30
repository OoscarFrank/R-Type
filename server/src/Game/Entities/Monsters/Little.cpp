#include "Little.hpp"
#include "../../Room.hpp"

LittleMonster::LittleMonster(Room &room, u_int id, short x, short y):
    Monster(room, id, x, y, LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT)
{
    _life = 100;
}

LittleMonster::LittleMonster(Room &room, u_int id, const std::pair<short, short> &pos):
    Monster(room, id, pos, {LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT})
{
    _life = 100;
}

void LittleMonster::refresh()
{
    auto now = std::chrono::system_clock::now();

    refreshMissiles();
    if (!_exist) {
        return;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= LITTLE_MONSTER_FIRE_TIME) {
        fireMissile(Missile::Type::LITTLE_MONSTER, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 0);
        _lastFire = now;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= MONSTER_MOVE_TIME) {
        move(-LITTLE_MONSTER_PROGRESS_STEP, 0);
        _room.sendToAll(StreamFactory::monsterPos(_id, LittleMonster::LITTLE_MONSTER, _box.x, _box.y));
        _lastMove = now;
    }
}

int LittleMonster::getDamage()
{
    return 10;
}
