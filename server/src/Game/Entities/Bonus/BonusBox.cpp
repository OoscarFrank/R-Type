#include "BonusBox.hpp"
#include "../../Room.hpp"
#include "../../../Utils/Stream.hpp"

BonusBox::BonusBox(Room &room, u_int id):
AEntity(room, id, 4000, (std::rand() % SCREEN_HEIGHT - 100) + 50, BONUS_WIDTH, BONUS_HEIGHT)
{
    _vx = -3;
    _vy = rand() % 6 - 3;
}

void BonusBox::refresh()
{
    auto now = std::chrono::system_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= BONUS_MOVE_TIME) {
        move(_vx, _vy);
        if (_box.x < 0)
            _exist = false;
        if (_box.y < 0 || _box.y > SCREEN_HEIGHT - _box.height)
            _vy = -_vy;
        this->_room.sendToAll(StreamFactory::bonusPos(_id, 1 ,_box.x, _box.y));
        _lastMove = now;
    }
}
