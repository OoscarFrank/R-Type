#include "Zigzager.hpp"
#include "../../Room.hpp"

ZigzagerMonster::ZigzagerMonster(Room &room, u_int id, short x, short y):
    Monster(room, id, x, y, LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT),
    _switch(1)
{
    _life = 150;
}

ZigzagerMonster::ZigzagerMonster(Room &room, u_int id, const std::pair<short, short> &pos):
    Monster(room, id, pos, {LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT}),
    _switch(1)
{
    _life = 150;
}

void ZigzagerMonster::refresh()
{
    auto now = std::chrono::system_clock::now();

    refreshMissiles();
    if (!_exist) {
        return;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= ENEMY_ZIGZAGER_FIRE_TIME) {
        fireMissile(Missile::Type::LITTLE_MONSTER, -7, 0);
        _lastFire = now;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= ENEMY_MOVE_TIME) {
        if (std::chrono::duration_cast<std::chrono::seconds>(now - _lastPos).count() >= 1) {
            _switch *= -1;
            _lastPos = now;
        }
        move(-1, _switch);
        _room.sendToAll(StreamFactory::monsterPos(_id, ZigzagerMonster::ZIGZAGER_MONSTER ,_box.x, _box.y));
        _lastMove = now;
    }
}

int ZigzagerMonster::getDamage()
{
    return 10;
}
