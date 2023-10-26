#include "Boss1.hpp"
#include "../../Room.hpp"

Boss1Monster::Boss1Monster(Room &room, u_int id, short x, short y):
    Monster(room, id, x, y, BOSS1_MONSTER_WIDTH, BOSS1_MONSTER_HEIGHT)
{
    _life = 100;
    _burstCount = 0;
}

Boss1Monster::Boss1Monster(Room &room, u_int id, const std::pair<short, short> &pos):
    Monster(room, id, pos, {BOSS1_MONSTER_WIDTH, BOSS1_MONSTER_HEIGHT})
{
    _life = 100;
    _burstCount = 0;
}

void Boss1Monster::refresh()
{
    auto now = std::chrono::system_clock::now();

    refreshMissiles();
    if (!_exist) {
        return;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= BURST_MONSTER_FIRE_TIME) {
        _burstCount = 0;
        _lastFire = now;
    }
    if (_burstCount <= 2) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= _burstCount * BURST_FIRE_TIME) {
            fireMissile(Missile::Type::LITTLE_MONSTER, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 0, _box.x + _box.width / 2, _box.y + _box.height / 2);
            fireMissile(Missile::Type::LITTLE_MONSTER, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 0, _box.x + _box.width / 2, _box.y + _box.height / 0.5);
            _burstCount++;
        }
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= MONSTER_MOVE_TIME) {
        move(-LITTLE_MONSTER_PROGRESS_STEP, 0);
        _room.sendToAll(StreamFactory::monsterPos(_id, IEntity::Type::BOSS1, _box.x, _box.y));
        _lastMove = now;
    }
}

int Boss1Monster::getDamage()
{
    return 10;
}
