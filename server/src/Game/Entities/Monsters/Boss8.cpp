#include "Boss8.hpp"
#include "../../Room.hpp"
#include <cmath>

Boss8Monster::Boss8Monster(Room &room, u_int id, short x, short y):
    Monster(room, id, x, y, BOSS8_MONSTER_WIDTH, BOSS8_MONSTER_HEIGHT)
{
    _life = 20000;
    _burstCount = 0;
    _moveDirection.first = 0;
    _moveDirection.second = 0;
    _vx = -BOSS2_MONSTER_PROGRESS_STEP;
    _vy = 0;
}

Boss8Monster::Boss8Monster(Room &room, u_int id, const std::pair<short, short> &pos):
    Monster(room, id, pos, {BOSS8_MONSTER_WIDTH, BOSS8_MONSTER_HEIGHT})
{
    _life = 20000;
    _burstCount = 0;
    _moveDirection.first = 0;
    _moveDirection.second = 0;
    _vx = -BOSS2_MONSTER_PROGRESS_STEP;
    _vy = 0;
}

void Boss8Monster::refresh()
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
    if (life() >= 16000) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= 2000) {
        _burstCount = 0;
        _lastFire = now;
        }
        if (_burstCount <= 5) {
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= _burstCount * BURST_FIRE_TIME) {
                fireMissile(Missile::Type::BOSS8_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 0, _box.x + _box.width / 2, _box.y + _box.height / 2);
                _lastFire = now;
                _burstCount++;
            }
        }
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= MONSTER_MOVE_TIME) {
            if (std::chrono::duration_cast<std::chrono::seconds>(now - _lastPos).count() >= CHANGE_HORIZONTAL_DIRECTION) {
                _moveDirection.first = std::rand() % 3 - 1;
                _lastPos = now;
            } else if (std::chrono::duration_cast<std::chrono::seconds>(now - _lastPos2).count() >= CHANGE_VERTICAL_DIRECTION) {
                _moveDirection.second = std::rand() % 2 - 1;
                _lastPos2 = now;
            }

            if (_box.x <= 850)
                _moveDirection.first = 1;
            else if (_box.x + _box.width > SCREEN_WIDTH)
                _moveDirection.first = -1;
            else if (_box.y + _box.height > SCREEN_HEIGHT)
                _moveDirection.second = -1;
            else if (_box.y <= 0)
                 _moveDirection.second = 1;

            const short moveX = _moveDirection.first * LITTLE_MONSTER_PROGRESS_STEP;
            const short moveY = _moveDirection.second * LITTLE_MONSTER_PROGRESS_STEP;
            move(moveX, moveY);
            if (std::chrono::duration_cast<std::chrono::seconds>(now - _teleport).count() >= 3) {
                _box.x = std::rand() % (SCREEN_WIDTH - 850 + 1) + 850;
                _box.y = std::rand() % SCREEN_HEIGHT;
                _teleport = now;
            }
            _room.sendToAll(StreamFactory::monsterPos(_id, IEntity::Type::BOSS8, _box.x, _box.y));
            _lastMove = now;
        }
    } else if(life() >= 13000 && life() < 16000) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= 1000) {
            fireMissile(Missile::Type::BOSS8_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 0, _box.x + _box.width / 2, _box.y + _box.height / 2);
            _lastFire = now;
        } else if(std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire2).count() >= 2000) {
            fireMissile(Missile::Type::FIRE_BALL, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 5, _box.x + _box.width / 2, _box.y + _box.height / 5.5);
            fireMissile(Missile::Type::FIRE_BALL, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, -5, _box.x + _box.width / 2, _box.y + _box.height / 3);
            fireMissile(Missile::Type::FIRE_BALL, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 15, _box.x + _box.width / 2, _box.y + _box.height / 5.5);
            fireMissile(Missile::Type::BOSS8_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, -15, _box.x + _box.width / 2, _box.y + _box.height / 3);
            fireMissile(Missile::Type::BOSS8_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 30, _box.x + _box.width / 2, _box.y + _box.height / 5.5);
            fireMissile(Missile::Type::BOSS8_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, -30, _box.x + _box.width / 2, _box.y + _box.height / 3);
            _lastFire2 = now;
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
            _room.sendToAll(StreamFactory::monsterPos(_id, IEntity::Type::BOSS8, _box.x, _box.y));
            _lastMove = now;
        }
    } else if(life() >= 2000 && life() < 13000) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= 6000) {
            _burstCount = 0;
            _lastFire = now;
        } else if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire2).count() >= 3000) {
            fireMissile(Missile::Type::FIRE_BALL, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 5, _box.x + _box.width / 2, _box.y + _box.height / 5.5);
            fireMissile(Missile::Type::FIRE_BALL, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, -5, _box.x + _box.width / 2, _box.y + _box.height / 3);
            fireMissile(Missile::Type::FIRE_BALL, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 15, _box.x + _box.width / 2, _box.y + _box.height / 5.5);
            fireMissile(Missile::Type::BOSS8_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, -15, _box.x + _box.width / 2, _box.y + _box.height / 3);
            fireMissile(Missile::Type::BOSS8_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 30, _box.x + _box.width / 2, _box.y + _box.height / 5.5);
            fireMissile(Missile::Type::BOSS8_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, -30, _box.x + _box.width / 2, _box.y + _box.height / 3);
            _lastFire2 = now;
        } else if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire3).count() >= 1000) {
            std::pair<double, double> tmpVect = vector;

            if (len != 0) {
                tmpVect.first /= len;
                tmpVect.first *= FOLLOWER_MONSTER_MISSILE_PROGRESS_STEP;
                tmpVect.second /= len;
                tmpVect.second *= FOLLOWER_MONSTER_MISSILE_PROGRESS_STEP;
            }
            fireMissile(Missile::Type::PURPLE_MISSILE, static_cast<short>(tmpVect.first), static_cast<short>(tmpVect.second));
            _lastFire3 = now;
        }
        if (_burstCount <= 5) {
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= _burstCount * BURST_FIRE_TIME) {
                const int numMissiles = 8;
                const double missileVelocity = 10.0;
                double angle = 0.0;

                for (int i = 0; i < numMissiles; ++i) {
                    fireMissile(Missile::Type::FIRE_BALL, missileVelocity * std::cos(angle), missileVelocity * std::sin(angle));
                    angle += 2.0 * MY_PI / numMissiles;
                }
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
            _room.sendToAll(StreamFactory::monsterPos(_id, IEntity::Type::BOSS8, _box.x, _box.y));
            _lastMove = now;
        }
    } else {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= 6000) {
            _burstCount = 0;
            _lastFire = now;
        } else if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire2).count() >= 3000) {
            fireMissile(Missile::Type::FIRE_BALL, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 5, _box.x + _box.width / 2, _box.y + _box.height / 5.5);
            fireMissile(Missile::Type::FIRE_BALL, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, -5, _box.x + _box.width / 2, _box.y + _box.height / 3);
            fireMissile(Missile::Type::FIRE_BALL, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 15, _box.x + _box.width / 2, _box.y + _box.height / 5.5);
            fireMissile(Missile::Type::BOSS8_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, -15, _box.x + _box.width / 2, _box.y + _box.height / 3);
            fireMissile(Missile::Type::BOSS8_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, 30, _box.x + _box.width / 2, _box.y + _box.height / 5.5);
            fireMissile(Missile::Type::BOSS8_MISSILE, -LITTLE_MONSTER_MISSILE_PROGRESS_STEP, -30, _box.x + _box.width / 2, _box.y + _box.height / 3);
            _lastFire2 = now;
        } else if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire3).count() >= 1000) {
            std::pair<double, double> tmpVect = vector;

            if (len != 0) {
                tmpVect.first /= len;
                tmpVect.first *= FOLLOWER_MONSTER_MISSILE_PROGRESS_STEP;
                tmpVect.second /= len;
                tmpVect.second *= FOLLOWER_MONSTER_MISSILE_PROGRESS_STEP;
            }
            fireMissile(Missile::Type::PURPLE_MISSILE, static_cast<short>(tmpVect.first), static_cast<short>(tmpVect.second));
            _lastFire3 = now;
        }
        if (_burstCount <= 5) {
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= _burstCount * BURST_FIRE_TIME) {
                const int numMissiles = 8;
                const double missileVelocity = 10.0;
                double angle = 0.0;

                for (int i = 0; i < numMissiles; ++i) {
                    fireMissile(Missile::Type::FIRE_BALL, missileVelocity * std::cos(angle), missileVelocity * std::sin(angle));
                    angle += 2.0 * MY_PI / numMissiles;
                }
                _burstCount++;
            }
        }
        if (_box.x < 0)
        _vx = BOSS2_MONSTER_PROGRESS_STEP;
        if (_box.x + _box.width > SCREEN_WIDTH)
            _vx = -BOSS2_MONSTER_PROGRESS_STEP;
        if (now - _lastMove >= std::chrono::milliseconds(MONSTER_MOVE_TIME)) {
            short bossY = _box.y + (_box.height / 2);
            short playerY = playerPos.second + (PLAYER_HEIGHT / 2);
            short bossX = _box.x + (_box.width / 2);
            short playerX = playerPos.first + (PLAYER_WIDTH / 2);
            _vy = (bossY == playerY || bossY + 1 == playerY || bossY - 1 == playerY) ? 0 : (bossY < playerY) ? BOSS2_MONSTER_PROGRESS_STEP : -BOSS2_MONSTER_PROGRESS_STEP;
            _vx = (bossX == playerX || bossX + 1 == playerX || bossX - 1 == playerX) ? 0 : (bossX < playerX) ? BOSS2_MONSTER_PROGRESS_STEP : -BOSS2_MONSTER_PROGRESS_STEP;
            move(_vx, _vy);
            _room.sendToAll(StreamFactory::monsterPos(_id, IEntity::Type::BOSS8, _box.x, _box.y));
            _lastMove = now;
        }
    }
}

int Boss8Monster::getDamage()
{
    if (life() >= 2000)
        return 10;
    else
        return 20;
}
