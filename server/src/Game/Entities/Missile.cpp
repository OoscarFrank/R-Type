#include "Missile.hpp"
#include "../Room.hpp"

Missile::Missile(Room &room, Missile::Type type, int id, short x, short y):
    AEntity(room, id, x, y, MISSILE_WIDTH, MISSILE_HEIGHT),
    _type(type)
{}

Missile::Missile(Room &room, Missile::Type type, int id, const std::pair<short, short> &pos):
    AEntity(room, id, pos, {MISSILE_WIDTH, MISSILE_HEIGHT}),
    _type(type)
{}

Missile::~Missile()
{
    Stream out;
    out.setDataUChar(15);
    out.setDataInt(_id);
    out.setDataUChar(_type);
    out.setDataShort(_box.x);
    out.setDataShort(_box.y);
    _room.sendToAll(out);
}

void Missile::refresh()
{
    auto now = std::chrono::system_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastMove).count() >= 5) { //TODO mettre un define pour le temps de déplacement
        move(_type == Missile::Type::ALLY ? ALLY_MISSILE_PROGRESS_STEP : -ENEMY_MISSILE_PROGRESS_STEP, 0);
        Stream out;
        out.setDataUChar(4);
        out.setDataInt(_id);
        out.setDataUChar(_type);
        out.setDataShort(_box.x);
        out.setDataShort(_box.y);
        _room.sendToAll(out);
        _lastMove = now;
    }
}
