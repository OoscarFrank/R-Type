#include "Boss1.hpp"
#include "../../Room.hpp"

Boss1Monster::Boss1Monster(Room &room, u_int id, short x, short y):
    Monster(room, id, x, y, BOSS1_MONSTER_WIDTH, BOSS1_MONSTER_HEIGHT)
{
    _life = 1000;
    _burst = std::chrono::system_clock::now();
    _burstSpam = std::chrono::system_clock::now();
    _changeVelocity = std::chrono::system_clock::now();
    _nbrBurst = 0;
    _vx = -BOSS1_MONSTER_PROGRESS_STEP;
    _vy = 0;
}

Boss1Monster::Boss1Monster(Room &room, u_int id, const std::pair<short, short> &pos):
    Monster(room, id, pos, {BOSS1_MONSTER_WIDTH, BOSS1_MONSTER_HEIGHT})
{
    _life = 1000;
    _burst = std::chrono::system_clock::now();
    _burstSpam = std::chrono::system_clock::now();
    _changeVelocity = std::chrono::system_clock::now();
    _nbrBurst = 0;
    _vx = -BOSS1_MONSTER_PROGRESS_STEP;
    _vy = 0;
}

void Boss1Monster::refresh()
{
    auto now = std::chrono::system_clock::now();
    const std::pair<short, short> playerPos = _room.getNearestPlayerPos(*this);

    refreshMissiles();
    if (!_exist) {
        return;
    }
    if (now - _lastFire >= std::chrono::milliseconds(BURST_MONSTER_FIRE_TIME)) {
        fireMissile(Missile::Type::LITTLE_MONSTER, -BOSS1_MONSTER_MISSILE_PROGRESS_STEP, 0, _box.x + _box.width / 2, _box.y + _box.height / 3);
        fireMissile(Missile::Type::LITTLE_MONSTER, -BOSS1_MONSTER_MISSILE_PROGRESS_STEP, 0, _box.x + _box.width / 2, _box.y + (_box.height / 3 * 2));
        _lastFire = now;
    }
    if (_box.x < 0)
        _vx = BOSS1_MONSTER_PROGRESS_STEP;
    if (_box.x + _box.width > SCREEN_WIDTH)
        _vx = -BOSS1_MONSTER_PROGRESS_STEP;
    if (now - _changeVelocity >= std::chrono::seconds(1)) {
        _vx = (std::rand() % BOSS1_MONSTER_PROGRESS_STEP) * (std::rand() % 2 ? -1 : 1);
        _changeVelocity = now;
    }
    if (now - _lastMove >= std::chrono::milliseconds(MONSTER_MOVE_TIME)) {
        short boss = _box.y + (_box.height / 2);
        short player = playerPos.second + (PLAYER_HEIGHT / 2);
        _vy = (boss == player || boss + 1 == player || boss - 1 == player) ? 0 : (boss < player) ? BOSS1_MONSTER_PROGRESS_STEP : -BOSS1_MONSTER_PROGRESS_STEP;
        move(_vx, _vy);
        _room.sendToAll(StreamFactory::monsterPos(_id, IEntity::Type::BOSS1, _box.x, _box.y));
        _lastMove = now;
    }
    _fireMissilesAllDirections();
}

int Boss1Monster::getDamage()
{
    return 10;
}

void Boss1Monster::_fireMissilesAllDirections()
{
    auto now = std::chrono::system_clock::now();
    if (now - _burst >= std::chrono::seconds(5)) {
        _nbrBurst = 5;
        _burst = now;
    }
    if (_nbrBurst && now - _burstSpam >= std::chrono::milliseconds(100)) {
        _nbrBurst--;
        fireMissile(Missile::Type::LITTLE_MONSTER, -BOSS1_MONSTER_MISSILE_PROGRESS_STEP, 0);
        fireMissile(Missile::Type::LITTLE_MONSTER, BOSS1_MONSTER_MISSILE_PROGRESS_STEP, 0);
        fireMissile(Missile::Type::LITTLE_MONSTER, 0, -BOSS1_MONSTER_MISSILE_PROGRESS_STEP);
        fireMissile(Missile::Type::LITTLE_MONSTER, 0, BOSS1_MONSTER_MISSILE_PROGRESS_STEP);
        _burstSpam = now;
    }
}
