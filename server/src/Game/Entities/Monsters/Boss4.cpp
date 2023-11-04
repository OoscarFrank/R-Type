#include "Boss4.hpp"
#include "../../Room.hpp"

Boss4Monster::Boss4Monster(Room &room, u_int id, short x, short y):
    Monster(room, id, x, y, BOSS1_MONSTER_WIDTH, BOSS1_MONSTER_HEIGHT)
{
    _life = 9000;
    _burstCount = 0;
    _nbrBurst = 0;
    moveDirection.first = 0;
    moveDirection.second = 0;
    _teleport = std::chrono::system_clock::now();
    _burst = std::chrono::system_clock::now();
    _burstSpam = std::chrono::system_clock::now();
}

Boss4Monster::Boss4Monster(Room &room, u_int id, const std::pair<short, short> &pos):
    Monster(room, id, pos, {BOSS1_MONSTER_WIDTH, BOSS1_MONSTER_HEIGHT})
{
    _life = 9000;
    _burstCount = 0;
    _nbrBurst = 0;
    moveDirection.first = 0;
    moveDirection.second = 0;
    _teleport = std::chrono::system_clock::now();
    _burst = std::chrono::system_clock::now();
    _burstSpam = std::chrono::system_clock::now();
}

void Boss4Monster::refresh()
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
            fireMissile(Missile::Type::ORANGE_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 0, _box.x + _box.width / 2, _box.y + _box.height / 3);
            fireMissile(Missile::Type::ORANGE_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 0, _box.x + _box.width / 2, _box.y + _box.height / 1.5);
            fireMissile(Missile::Type::GREEN_MISSILE, -BOSS2_MONSTER_MISSILE_PROGRESS_STEP, BOSS2_MONSTER_MISSILE_PROGRESS_STEP);
            fireMissile(Missile::Type::GREEN_MISSILE, -BOSS2_MONSTER_MISSILE_PROGRESS_STEP, -BOSS2_MONSTER_MISSILE_PROGRESS_STEP);
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

        if (_box.x <= 850)
            moveDirection.first = 1;
        else if (_box.x + _box.width > SCREEN_WIDTH)
            moveDirection.first = -1;
        else if (_box.y + _box.height > SCREEN_HEIGHT)
            moveDirection.second = -1;
        else if (_box.y <= 0)
             moveDirection.second = 1;

        const short moveX = moveDirection.first * LITTLE_MONSTER_PROGRESS_STEP;
        const short moveY = moveDirection.second * LITTLE_MONSTER_PROGRESS_STEP;
        move(moveX, moveY);
        if (std::chrono::duration_cast<std::chrono::seconds>(now - _teleport).count() >= 5) {
            _box.x = std::rand() % (SCREEN_WIDTH - 850 + 1) + 850;
            _box.y = std::rand() % SCREEN_HEIGHT;
            _teleport = now;
        }
        _room.sendToAll(StreamFactory::monsterPos(_id, IEntity::Type::BOSS4, _box.x, _box.y));
        _lastMove = now;
    }
    _fireMissilesAllDirections();
}

int Boss4Monster::getDamage()
{
    return 10;
}

void Boss4Monster::_fireMissilesAllDirections()
{
    auto now = std::chrono::system_clock::now();
    if (now - _burst >= std::chrono::milliseconds(5100)) {
        _nbrBurst = 5;
        _burst = now;
    }
    if (_nbrBurst && now - _burstSpam >= std::chrono::milliseconds(100)) {
        _nbrBurst--;
        fireMissile(Missile::Type::LITTLE_MONSTER, -BOSS2_MONSTER_MISSILE_PROGRESS_STEP, 0);
        fireMissile(Missile::Type::LITTLE_MONSTER, BOSS2_MONSTER_MISSILE_PROGRESS_STEP, 0);
        fireMissile(Missile::Type::LITTLE_MONSTER, 0, -BOSS2_MONSTER_MISSILE_PROGRESS_STEP);
        fireMissile(Missile::Type::LITTLE_MONSTER, 0, BOSS2_MONSTER_MISSILE_PROGRESS_STEP);
        fireMissile(Missile::Type::LITTLE_MONSTER, -BOSS2_MONSTER_MISSILE_PROGRESS_STEP, -BOSS2_MONSTER_MISSILE_PROGRESS_STEP);
        fireMissile(Missile::Type::LITTLE_MONSTER, -BOSS2_MONSTER_MISSILE_PROGRESS_STEP, BOSS2_MONSTER_MISSILE_PROGRESS_STEP);
        fireMissile(Missile::Type::LITTLE_MONSTER, BOSS2_MONSTER_MISSILE_PROGRESS_STEP, -BOSS2_MONSTER_MISSILE_PROGRESS_STEP);
        fireMissile(Missile::Type::LITTLE_MONSTER, BOSS2_MONSTER_MISSILE_PROGRESS_STEP, BOSS2_MONSTER_MISSILE_PROGRESS_STEP);
        _burstSpam = now;
    }
}
