#include "Missiles.hpp"
#include "../Utils/Scheduling.hpp"
#include "../Utils/Instruction.hpp"
#include "Room.hpp"

Missiles::Missiles(unsigned short x, unsigned short y, int id, unsigned char type, Room &room) : _room(room)
{
    _x = x;
    _y = y;
    _id = id;
    _type = type;
}

Missiles::~Missiles()
{

}


bool Missiles::Refresh()
{
    if (_x > SCREEN_WIDTH) {
        _room.setInstBroadcast(15);
        _room.getBroadcastStream().setDataInt(_id);
        _room.getBroadcastStream().setDataUChar(_type);
        _room.getBroadcastStream().setDataShort(_x);
        _room.getBroadcastStream().setDataShort(_y);
        _room.sendBroadcast();
        return false;
    } else {
        _x += MISSILE_PROGRESS_STEP;
        _room.setInstBroadcast(4);
        _room.getBroadcastStream().setDataInt(_id);
        _room.getBroadcastStream().setDataUChar(_type);
        _room.getBroadcastStream().setDataShort(_x);
        _room.getBroadcastStream().setDataShort(_y);
        _room.sendBroadcast();
        return true;
    }
}