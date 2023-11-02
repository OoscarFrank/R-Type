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
    auto now = std::chrono::system_clock::now();
    if (_player.exists() && std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastBomb).count() >= PLAYER_FIRE_BOMB_TIME) {
        _bombs.push_back(std::make_unique<Bomb>(_room, ++_room.getBombIds(), _player.box().x + _player.box().width / 2, _player.box().y, _isFront));
    }
}

void ForcePod::bombCollide(IEntity &other)
{
    for(auto i = _bombs.begin(); i != _bombs.end(); ++i) {
        if ((*i)->collide(other)) {
            other.setLife(other.life() - BOMB_DAMAGE);
            _player.setScore(_player.score() + 15);
            _room.sendToAll(StreamFactory::bombDestroyed((*i)->id()));
            i = _bombs.erase(i);
            if (i == _bombs.end())
                break;
        }
    }
}