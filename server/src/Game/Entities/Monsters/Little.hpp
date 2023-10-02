#pragma once

#include "../Entity.hpp"
#include "../ArmedEntity.hpp"

class LittleMonster: public ArmedEntity {
    public:
        LittleMonster(Room &room, u_int id, short x, short y);
        LittleMonster(Room &room, u_int id, const std::pair<short, short> &pos);
        virtual ~LittleMonster();

        virtual void refresh();
        virtual bool collide(const IEntity &other) override;
};
