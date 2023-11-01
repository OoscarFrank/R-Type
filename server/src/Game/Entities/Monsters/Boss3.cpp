#include "Boss3.hpp"
#include "../../Room.hpp"
#include <cmath>

Boss3Monster::Boss3Monster(Room &room, u_int id, short x, short y):
    Monster(room, id, x, y, BOSS1_MONSTER_WIDTH, BOSS1_MONSTER_HEIGHT),
    _switch(1)
{
    _life = 1500;
    _burstCount = 0;
    moveDirection.first = 0;
    moveDirection.second = 0;
    _changeVelocity = std::chrono::system_clock::now();
    _vx = -BOSS2_MONSTER_PROGRESS_STEP;
    _vy = 0;
}

Boss3Monster::Boss3Monster(Room &room, u_int id, const std::pair<short, short> &pos):
    Monster(room, id, pos, {BOSS1_MONSTER_WIDTH, BOSS1_MONSTER_HEIGHT}),
    _switch(1)
{
    _life = 1500;
    _burstCount = 0;
    moveDirection.first = 0;
    moveDirection.second = 0;
    _changeVelocity = std::chrono::system_clock::now();
    _vx = -BOSS2_MONSTER_PROGRESS_STEP;
    _vy = 0;
}

void Boss3Monster::refresh()
{
    auto now = std::chrono::system_clock::now();
    std::pair<short, short> nearestPlayer = _room.getNearestPlayerPos(*this);
    std::pair<short, short> startPoint = {_box.x + _box.width / 2, _box.y + _box.height / 2};
    const std::pair<short, short> playerPos = _room.getNearestPlayerPos(*this);
    std::pair<double, double> vector = {nearestPlayer.first - startPoint.first, nearestPlayer.second - startPoint.second};
    double len = std::sqrt(std::pow(vector.first, 2) + std::pow(vector.second, 2));

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
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastPosFire).count() >= 100) {
            _switch *= -1;
            _lastPosFire = now;
            }
            fireMissile(Missile::Type::ORANGE_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, _switch * 10, _box.x + _box.width / 2, _box.y + _box.height / 3);
            fireMissile(Missile::Type::ORANGE_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, _switch * 10, _box.x + _box.width / 2, _box.y + _box.height / 1.5);
            fireMissile(Missile::Type::ORANGE_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 0, _switch * (_box.x + _box.width / 2), _box.y + _box.height / 3);
            fireMissile(Missile::Type::ORANGE_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 0, _box.x + _box.width / 2, _box.y + _box.height / 1.5);
            _burstCount++;
        }
    }
    if (_box.x < 0)
        _vx = BOSS2_MONSTER_PROGRESS_STEP;
    if (_box.x + _box.width > SCREEN_WIDTH)
        _vx = -BOSS2_MONSTER_PROGRESS_STEP;
    if (now - _changeVelocity >= std::chrono::seconds(1)) {
        _vx = (std::rand() % BOSS2_MONSTER_PROGRESS_STEP) * (std::rand() % 2 ? -1 : 1);
        _changeVelocity = now;
    }
    if (now - _lastMove >= std::chrono::milliseconds(MONSTER_MOVE_TIME)) {
        short boss = _box.y + (_box.height / 2);
        short player = playerPos.second + (PLAYER_HEIGHT / 2);
        _vy = (boss == player || boss + 1 == player || boss - 1 == player) ? 0 : (boss < player) ? BOSS2_MONSTER_PROGRESS_STEP : -BOSS2_MONSTER_PROGRESS_STEP;
        move(_vx, _vy);
        _room.sendToAll(StreamFactory::monsterPos(_id, IEntity::Type::BOSS3, _box.x, _box.y));
        _lastMove = now;
    }
}

int Boss3Monster::getDamage()
{
    return 10;
}
