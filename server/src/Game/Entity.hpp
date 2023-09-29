#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include "../Client.hpp"
#include "../Utils/Scheduling.hpp"
#include "../Utils/Instruction.hpp"

class Room;

namespace Entities {
    class IEntity {
        public:
            virtual ~IEntity() = default;

            virtual void refresh() = 0;
            virtual const std::pair<short, short> &position() const = 0;
            virtual int id() const = 0;
            virtual bool isOutOfScreen() const = 0;

            enum Type {
                MISSILE,
                PLAYER,
                LITTLE_MONSTER,
            };

            // static std::unique_ptr<IEntity> create(Type type, Room &room, int id, short x, short y);
    };

    class AEntity: public IEntity {
        public:
            AEntity(Room &room, int id, short x, short y);
            AEntity(Room &room, int id, const std::pair<short, short> &pos);
            virtual ~AEntity() = default;

            virtual void refresh() = 0;
            virtual const std::pair<short, short> &position() const;
            virtual int id() const;
            virtual bool isOutOfScreen() const;

        protected:
            virtual void move(short dx, short dy);

            Room &_room;
            int _id;
            std::pair<short, short> _pos;
            std::chrono::system_clock::time_point _lastMove;
    };

    class Missile: public AEntity {
        public:
            enum Type {
                ALLY = 1,
                ENEMY = 2,
            };

            Missile(Room &room, Missile::Type type, int id, short x, short y);
            Missile(Room &room, Missile::Type type, int id, const std::pair<short, short> &pos);
            virtual ~Missile();

            virtual void refresh();

        private:
            Missile::Type _type;
    };

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

    class Player: public ArmedEntity {
        public:
            Player(Room &room, std::shared_ptr<Client> client, int id, short x, short y);
            Player(Room &room, std::shared_ptr<Client> client, int id, const std::pair<short, short> &pos);
            virtual ~Player();

            virtual void refresh();
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

    class LittleMonster: public ArmedEntity {
        public:
            LittleMonster(Room &room, int id, short x, short y);
            LittleMonster(Room &room, int id, const std::pair<short, short> &pos);
            virtual ~LittleMonster() = default;

            virtual void refresh();
    };
}
