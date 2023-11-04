#include "ForcePod.hpp"
#include "Entities/Player.hpp"
#include "Room.hpp"

ForcePod::ForcePod(Room &room, Player &player)
    :_room(room), _player(player) ,_isFront(false)
{
    
}

void ForcePod::refresh()
{
    for(auto i = _bombs.begin(); i != _bombs.end(); ++i) {
        (*i)->refresh();
        if (!(*i)->exists()) {
            _room.sendToAll(StreamFactory::bombDestroyed((*i)->id()));
            _room.degInZone((*i)->box().x + (*i)->box().width / 2, (*i)->box().y + (*i)->box().height / 2, 256, this->_player);
            i = _bombs.erase(i);
            if (i == _bombs.end())
                break;
        }
    }

    for(auto i = _lasers.begin(); i != _lasers.end(); ++i) {
        (*i)->refresh();
        if (!(*i)->exists()) {
            i = _lasers.erase(i);
            if (i == _lasers.end())
                break;
        }
    }

    for(auto i = _rays.begin(); i != _rays.end(); ++i) {
        (*i)->refresh();
        if (!(*i)->exists()) {
            i = _rays.erase(i);
            if (i == _rays.end())
                break;
        }
    }
}

void ForcePod::toggleFront()
{
    _isFront = !_isFront;
}

void ForcePod::shootBomb()
{
    if (this->lvl < 1)
        return;
    auto now = std::chrono::system_clock::now();
    if (_player.exists() && std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastBomb).count() >= PLAYER_FIRE_BOMB_TIME) {
        _bombs.push_back(std::make_unique<Bomb>(_room, ++_room.getBombIds(), _player.box().x + _player.box().width / 2, _player.box().y, _isFront));
        _lastBomb = now;
    }
}

void ForcePod::bombCollide(IEntity &other)
{
    for(auto i = _bombs.begin(); i != _bombs.end(); ++i) {
        if ((*i)->collide(other)) {
            other.setLife(other.life() - BOMB_DAMAGE);
            _player.setScore(_player.score() + BOMB_SCORE);
            _room.sendToAll(StreamFactory::bombDestroyed((*i)->id()));
            _room.degInZone((*i)->box().x + (*i)->box().width / 2, (*i)->box().y + (*i)->box().height / 2, 256, this->_player);
            i = _bombs.erase(i);
            if (i == _bombs.end())
                break;
        }
    }

    
}

void ForcePod::setLvl(u_char lvl)
{
    if (this->lvl < 3)
        this->lvl++;
}

u_char ForcePod::getLvl() const
{
    return lvl;
}

void ForcePod::shootLaser()
{
    if (this->lvl < 2)
        return;
    auto now = std::chrono::system_clock::now();
    if (_player.exists() && std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastLaser).count() >= PLAYER_FIRE_LASER_TIME) {
        _lasers.push_back(std::make_unique<Laser>(_room, ++_room.getLaserIds(), 0, _player.box().y - (LASER_HEIGHT - _player.box().height) / 2, _isFront));
        _lastLaser = now;
    }
}

void ForcePod::shootRay()
{
    if (this->lvl < 3)
        return;
     auto now = std::chrono::system_clock::now();
    if (_player.exists() && std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastRay).count() >= PLAYER_FIRE_RAY_TIME) {
        _rays.push_back(std::make_unique<Ray>(_room, ++_room.getRayIds(), _player.box().x + _player.box().width / 2, _player.box().y + _player.box().height / 2, _isFront, true));
        _rays.push_back(std::make_unique<Ray>(_room, ++_room.getRayIds(), _player.box().x + _player.box().width / 2, _player.box().y + _player.box().height / 2, _isFront, false));
        _lastRay = now;
    }
}

void ForcePod::laserCollide(IEntity &other)
{

    for(auto i = _lasers.begin(); i != _lasers.end(); ++i) {
        if ((*i)->collide(other)) {
            other.setLife(other.life() - LASER_DAMAGE);
            _player.setScore(_player.score() + LASER_SCORE);
        }
    }
}

void ForcePod::rayCollide(IEntity &other)
{
    for(auto i = _rays.begin(); i != _rays.end(); ++i) {
        if ((*i)->collide(other)) {
            other.setLife(other.life() - RAY_DAMAGE);
            _player.setScore(_player.score() + RAY_SCORE);
        }
    }
}