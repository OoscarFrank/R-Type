#pragma once


class Room;


class Missiles
{
public:
    enum missiles {
        ALlY,
        ENEMY,
    };
    Missiles(unsigned short x, unsigned short y, int id, unsigned char type, Room &room);
    ~Missiles();
    bool Refresh(/* mettre tout les éléments de colision (ennemies) */);

private:
    int _x;
    int _y;
    int _id;
    unsigned char _type;
    Room &_room;
};
