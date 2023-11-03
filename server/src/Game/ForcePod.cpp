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