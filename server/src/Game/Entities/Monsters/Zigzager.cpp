#include "Zigzager.hpp"
#include "../../Room.hpp"

ZigzagerMonster::ZigzagerMonster(Room &room, u_int id, short x, short y):
    Monster(room, id, x, y, ZIGZAGER_MONSTER_WIDTH, ZIGZAGER_MONSTER_HEIGHT),
    _lastPos(std::chrono::system_clock::now() - std::chrono::milliseconds(500)),
    _switch(1)
{
    _life = 100;
}

ZigzagerMonster::ZigzagerMonster(Room &room, u_int id, const std::pair<short, short> &pos):
    Monster(room, id, pos, {ZIGZAGER_MONSTER_WIDTH, ZIGZAGER_MONSTER_HEIGHT}),
    _lastPos(std::chrono::system_clock::now() - std::chrono::milliseconds(500)),
    _switch(1)
{
    _life = 100;
}

void ZigzagerMonster::refresh()
{
    auto now = std::chrono::system_clock::now();

    refreshMissiles();
    if (!_exist) {
        return;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= ZIGZAGER_MONSTER_FIRE_TIME) {
        fireMissile(Missile::Type::LITTLE_MONSTER, -ZIGZAGER_MONSTER_MISSILE_PROGRESS_STEP, 0);
        _lastFire = now;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= MONSTER_MOVE_TIME) {
        if (std::chrono::duration_cast<std::chrono::seconds>(now - _lastPos).count() >= 1) {
            _switch *= -1;
            _lastPos = now;
        }
        move(-ZIGZAGER_MONSTER_PROGRESS_STEP, _switch * ZIGZAGER_MONSTER_PROGRESS_STEP_VERTICAL);
        _room.sendToAll(StreamFactory::monsterPos(_id, ZigzagerMonster::ZIGZAGER_MONSTER ,_box.x, _box.y));
        _lastMove = now;
    }
}

int ZigzagerMonster::getDamage()
{
    return 10;
}
