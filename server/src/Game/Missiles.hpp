#pragma once

#include <cstddef>

class Room;


class Missiles
{
public:
    enum missiles {
        ALlY = 1,
        ENEMY = 2,
    };
    Missiles(unsigned short x, unsigned short y, size_t id, unsigned char type, Room &room);
    ~Missiles();
    bool Refresh(/* mettre tout les éléments de colision (ennemies) */);

private:
    int _x;
    int _y;
    size_t _id;
    unsigned char _type;
    Room &_room;
};
