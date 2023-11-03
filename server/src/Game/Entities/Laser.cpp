#include "Laser.hpp"
#include "../Room.hpp"

Laser::Laser(Room &room, u_int id, short x, short y, bool &front):
AEntity(room, id, x, y, SCREEN_WIDTH , LASER_HEIGHT),
_front(front)
{
    _counter = 0;
    _room.sendToAll(StreamFactory::laserCreated(id, y));
}


void Laser::refresh()
{
    auto now = std::chrono::system_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= LASER_MOVE_TIME) {
        if (_counter > 19) {
            _exist = false;
        }
        ++_counter;
        _lastMove = now;
    }
}
