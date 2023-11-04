#include "Ray.hpp"
#include "../Room.hpp"

Ray::Ray(Room &room, u_int id, short x, short y, bool &front, bool top):
AEntity(room, id, x, y, RAY_HEIGHT , RAY_WIDTH),
_front(front)
{
    _vx = 20;
    if (top)
        _vy = 25;
    else
        _vy = -25;
    _counter = 0;
}

void Ray::refresh()
{
    auto now = std::chrono::system_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= RAY_MOVE_TIME) {
        move(_vx, _vy);
        if (this->_box.x < 0 || this->_box.x > SCREEN_WIDTH - RAY_WIDTH) {
            _vx = -_vx;
            ++_counter;
        }
        if (this->_box.y < 0 || this->_box.y > SCREEN_HEIGHT - RAY_HEIGHT) {
            _vy = -_vy;
            ++_counter;
        }

        if (_counter > 2) {
            _exist = false;
        }
        if (_box.x < 0)
            _box.x = 0;
        if (_box.x > SCREEN_WIDTH - RAY_WIDTH)
            _box.x = SCREEN_WIDTH - RAY_WIDTH;
        if (_box.y < 0)
            _box.y = 0;
        if (_box.y > SCREEN_HEIGHT - RAY_HEIGHT)
            _box.y = SCREEN_HEIGHT - RAY_HEIGHT;
        
        this->_room.sendToAll(StreamFactory::rayPos(_id, _box.x, _box.y));

        _lastMove = now;
    }
}
