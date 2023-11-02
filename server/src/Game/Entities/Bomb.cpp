#include "Bomb.hpp"
#include "../Room.hpp"

Bomb::Bomb(Room &room, u_int id, short x, short y, bool &front) :
AEntity(room, id, x, y, BOMB_WIDTH, BOMB_HEIGHT),
_front(front)
{
    _vx = _front ? -16 : 16;
    _yCounter = -20;
}

void Bomb::refresh()
{
    auto now = std::chrono::system_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= BOMB_MOVE_TIME) {
        short vy = _yCounter < 15 ? _yCounter : 15;
        move(_vx, _yCounter);
        _yCounter++;
        if (_yCounter == 25)
            _vx = _front ? -13 : 13;
        if (_yCounter == 35)
            _vx = _front ? -9 : 9;
        if (_yCounter == 50)
            _vx = _front ? -5 : 5;
        if (_box.x < 0 || _box.x > SCREEN_WIDTH || _box.y > SCREEN_HEIGHT)
            _exist = false;
        if (_exist)
            this->_room.sendToAll(StreamFactory::bombPos(_id, _box.x, _box.y));
        _lastMove = now;
    }
}