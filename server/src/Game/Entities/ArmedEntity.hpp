#pragma once

#include "Entity.hpp"
#include "Missile.hpp"

class ArmedEntity: public AEntity {
    public:
        ArmedEntity(Room &room, int id, short x, short y);
        ArmedEntity(Room &room, int id, const std::pair<short, short> &pos);
        virtual ~ArmedEntity() = default;

        virtual void refresh() = 0;

    protected:
        virtual void refreshMissiles();
        virtual void fireMissile(Missile::Type type);
        std::chrono::system_clock::time_point _lastFire;

    private:
        std::vector<std::unique_ptr<Missile>> _missiles;
        std::mutex _missilesMutex;
};
