#pragma once

#include "Entity.hpp"
#include "ArmedEntity.hpp"

class Player: public ArmedEntity {
    public:
        Player(Room &room, std::shared_ptr<Client> client, int id, short x, short y);
        Player(Room &room, std::shared_ptr<Client> client, int id, const std::pair<short, short> &pos);
        virtual ~Player();

        virtual void refresh();
        virtual bool collide(const IEntity &other) override;
        virtual void move(short dx, short dy);
        virtual void sendPos();
        virtual void fireMissile();

        const std::chrono::system_clock::time_point &lastMoveTime() const;
        void setLastMoveTime(const std::chrono::system_clock::time_point &lastMoveTime);
        int score() const;
        void setScore(int score);
        std::shared_ptr<Client> client() const;

    private:
        int _score;
        std::shared_ptr<Client> _client;
};
