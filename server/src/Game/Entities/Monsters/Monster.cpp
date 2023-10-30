#include "Monster.hpp"
#include "../../Room.hpp"

Monster::Monster(Room &room, u_int id, short x, short y, short w, short h):
    ArmedEntity(room, id, x, y, w, h)
{}

Monster::Monster(Room &room, u_int id, const std::pair<short, short> &pos, const std::pair<short, short> &size):
    ArmedEntity(room, id, pos, size)
{}

bool Monster::collide(const IEntity &other)
{
    return AEntity::collide(other) || missilesCollide(other);
}

void Monster::setLife(int life)
{
    AEntity::setLife(life);
    _room.sendToAll(StreamFactory::monsterLife(_id, _life));
    if (_life <= 0)
        kill();
}

void Monster::kill()
{
    AEntity::kill();
    _room.sendToAll(StreamFactory::monsterDied(_id));
}
