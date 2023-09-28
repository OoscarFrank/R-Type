#include "Little.hpp"

Little::Little(Room &room, size_t id, int x, int y):
    AMonster(room, id, x, y),
    _lastFire(std::chrono::system_clock::now()),
    _lastMove(std::chrono::system_clock::now())
{}

void Little::refresh()
{
    auto now = std::chrono::system_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFire).count() >= 1000) {
        fireMissile();
        _lastFire = now;
    }
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= 2) {
        move(-1, 0);
        _lastMove = now;
        for (auto i = _missiles.begin(); i != _missiles.end();) {
            if (!(**i).Refresh())
                i = _missiles.erase(i);
            else
                i++;
        }
    }

}
