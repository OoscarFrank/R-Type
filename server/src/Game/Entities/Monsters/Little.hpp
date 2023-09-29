#pragma once

#include "../Entity.hpp"
#include "../ArmedEntity.hpp"

class LittleMonster: public ArmedEntity {
    public:
        LittleMonster(Room &room, int id, short x, short y);
        LittleMonster(Room &room, int id, const std::pair<short, short> &pos);
        virtual ~LittleMonster() = default;

        virtual void refresh();
        virtual bool collide(const IEntity &other) override;
};
