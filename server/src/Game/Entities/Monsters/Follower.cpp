#include "Follower.hpp"
#include "../../Room.hpp"
#include <cmath>

FollowerMonster::FollowerMonster(Room &room, u_int id, short x, short y):
    ArmedEntity(room, id, x, y, LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT)
{}

FollowerMonster::FollowerMonster(Room &room, u_int id, const std::pair<short, short> &pos):
    ArmedEntity(room, id, pos, {LITTLE_MONSTER_WIDTH, LITTLE_MONSTER_HEIGHT})
{}

FollowerMonster::~FollowerMonster()
{

}

void FollowerMonster::refresh()
{
    auto now = std::chrono::system_clock::now();
    std::pair<short, short> nearestPlayer = _room.getNearestPlayerPos(*this);
    std::pair<short, short> startPoint = {_box.x + _box.width / 2, _box.y + _box.height / 2};
    std::pair<double, double> vector = {nearestPlayer.first - startPoint.first, nearestPlayer.second - startPoint.second};
    double len = std::sqrt(std::pow(vector.first, 2) + std::pow(vector.second, 2));

    refreshMissiles();
    if (!_exist) {
        return;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= ENEMY_LITTLE_FIRE_TIME) {
        std::pair<double, double> tmpVect = vector;
        if (len != 0) {
            tmpVect.first /= len;
            tmpVect.first *= ENEMY_MISSILE_PROGRESS_STEP;
            tmpVect.second /= len;
            tmpVect.second *= ENEMY_MISSILE_PROGRESS_STEP;
        }
        fireMissile(Missile::Type::LITTLE_MONSTER, static_cast<short>(tmpVect.first), static_cast<short>(tmpVect.second));
        _lastFire = now;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= ENEMY_MOVE_TIME) {
        std::pair<double, double> tmpVect = vector;
        if (len != 0) {
            tmpVect.first /= len;
            tmpVect.first *= 2;
            tmpVect.second /= len;
            tmpVect.second *= 2;
        }
        move(tmpVect.first, tmpVect.second);
        _room.sendToAll(StreamFactory::monsterPos(_id, IEntity::Type::FOLLOWER_MONSTER, _box.x, _box.y));
        _lastMove = now;
    }
}

bool FollowerMonster::collide(const IEntity &other)
{
    return AEntity::collide(other) || missilesCollide(other);
}

void FollowerMonster::setLife(int life)
{
    AEntity::setLife(life);
    _room.sendToAll(StreamFactory::monsterLife(_id, _life));
}

void FollowerMonster::removeHP()
{
    setLife(_life - 100);
}
