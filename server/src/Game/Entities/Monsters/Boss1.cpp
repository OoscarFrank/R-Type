#include "Boss1.hpp"
#include "../../Room.hpp"

Boss1Monster::Boss1Monster(Room &room, u_int id, short x, short y):
    Monster(room, id, x, y, BOSS1_MONSTER_WIDTH, BOSS1_MONSTER_HEIGHT)
{
    _life = 6500;
    _burstCount = 0;
    moveDirection.first = 0;
    moveDirection.second = 0;
}

Boss1Monster::Boss1Monster(Room &room, u_int id, const std::pair<short, short> &pos):
    Monster(room, id, pos, {BOSS1_MONSTER_WIDTH, BOSS1_MONSTER_HEIGHT})
{
    _life = 6500;
    _burstCount = 0;
    moveDirection.first = 0;
    moveDirection.second = 0;
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
    if (_burstCount <= 3) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= _burstCount * BURST_FIRE_TIME) {
            fireMissile(Missile::Type::PURPLE_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 0, _box.x + _box.width / 2, _box.y + _box.height / 3);
            fireMissile(Missile::Type::PURPLE_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 0, _box.x + _box.width / 2, _box.y + _box.height / 1.5);
            fireMissile(Missile::Type::PURPLE_MISSILE, -BOSS2_MONSTER_MISSILE_PROGRESS_STEP, BOSS2_MONSTER_MISSILE_PROGRESS_STEP);
            fireMissile(Missile::Type::PURPLE_MISSILE, -BOSS2_MONSTER_MISSILE_PROGRESS_STEP, -BOSS2_MONSTER_MISSILE_PROGRESS_STEP);
            _burstCount++;
        }
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= MONSTER_MOVE_TIME) {
        if (std::chrono::duration_cast<std::chrono::seconds>(now - _lastPos).count() >= CHANGE_HORIZONTAL_DIRECTION) {
            moveDirection.first = std::rand() % 3 - 1;
            _lastPos = now;
        } else if (std::chrono::duration_cast<std::chrono::seconds>(now - _lastPos2).count() >= CHANGE_VERTICAL_DIRECTION) {
            moveDirection.second = std::rand() % 2 - 1;
            _lastPos2 = now;
        }

        if (_box.x == 850)
            moveDirection.first = 1;
        else if (_box.x + _box.width > SCREEN_WIDTH)
            moveDirection.first = -1;
        else if (_box.y + _box.height > SCREEN_HEIGHT)
            moveDirection.second = -1;
        else if (_box.y == 0)
             moveDirection.second = 1;

        const short moveX = moveDirection.first * LITTLE_MONSTER_PROGRESS_STEP;
        const short moveY = moveDirection.second * LITTLE_MONSTER_PROGRESS_STEP;
        move(moveX, moveY);
        _room.sendToAll(StreamFactory::monsterPos(_id, IEntity::Type::BOSS1, _box.x, _box.y));
        _lastMove = now;
    }
}

int Boss1Monster::getDamage()
{
    return 10;
}
