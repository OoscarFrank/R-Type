#include "BurstFire.hpp"
#include "../../Room.hpp"

BurstMonster::BurstMonster(Room &room, u_int id, short x, short y):
    Monster(room, id, x, y, LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT)
{
    _life = 100;
    _burstCount = 0;
}

BurstMonster::BurstMonster(Room &room, u_int id, const std::pair<short, short> &pos):
    Monster(room, id, pos, {LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT})
{
    _life = 100;
    _burstCount = 0;
}

void BurstMonster::refresh()
{
    auto now = std::chrono::system_clock::now();

    refreshMissiles();
    if (!_exist) {
        return;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= 1000) {
        _burstCount = 0;
        _lastFire = now;
    }
    if (_burstCount < 3) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= _burstCount * 200) {
            fireMissile(Missile::Type::LITTLE_MONSTER, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 0);
            _burstCount++;
        }
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= MONSTER_MOVE_TIME) {
        move(-LITTLE_MONSTER_PROGRESS_STEP, 0);
        _room.sendToAll(StreamFactory::monsterPos(_id, IEntity::Type::BURST_MONSTER, _box.x, _box.y));
        _lastMove = now;
    }
}

int BurstMonster::getDamage()
{
    return 10;
}
