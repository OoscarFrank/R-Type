#ifndef ARMEDENTITY_HPP
#define ARMEDENTITY_HPP

#include "Entity.hpp"
#include "Missile.hpp"

class ArmedEntity: public AEntity {
    public:
        ArmedEntity(Room &room, u_int id, short x, short y, short w, short h);
        ArmedEntity(Room &room, u_int id, const std::pair<short, short> &pos, const std::pair<short, short> &size);
        virtual ~ArmedEntity() = default;

        virtual void refresh() = 0;

    protected:
        virtual void refreshMissiles();
        virtual bool missilesCollide(const IEntity &other);
        virtual void fireMissile(Missile::Type type);
        std::chrono::system_clock::time_point _lastFire;

    private:
        std::vector<std::unique_ptr<Missile>> _missiles;
        std::mutex _missilesMutex;
};

#endif
