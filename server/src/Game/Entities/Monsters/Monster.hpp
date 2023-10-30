#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "../Entity.hpp"
#include "../ArmedEntity.hpp"

class Monster: public ArmedEntity {
    public:
        Monster(Room &room, u_int id, short x, short y, short w, short h);
        Monster(Room &room, u_int id, const std::pair<short, short> &pos, const std::pair<short, short> &size);
        virtual ~Monster() = default;

        virtual void refresh() = 0;
        virtual bool collide(const IEntity &other) override;
        virtual void setLife(int life) override;
        virtual void kill() override;
        virtual int getDamage() = 0;

    protected:
    private:
};

#endif
