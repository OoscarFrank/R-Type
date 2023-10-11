#ifndef ROOM_HPP
#define ROOM_HPP

#include <iostream>
#include <vector>
#include <thread>
#include "Entities/Entity.hpp"
#include "Entities/Player.hpp"
#include "Entities/Monsters/Little.hpp"
#include "../Client.hpp"

class Room
{
    private:
        enum State {
            WAIT = 0,
            RUN = 1,
            END = 2,
        };

        unsigned char _state;
        std::thread _thread;
        std::mutex _playersMutex;
        std::vector<std::unique_ptr<Player>> _players;
        std::vector<std::unique_ptr<IEntity>> _monsters;
        u_int _id;
        unsigned int _maxPlayer;
        unsigned int _progress;
        u_int _playersIds;
        bool _private;

        u_int _missilesIds;
        u_int _monstersIds;

        Stream _broadcastStream;
        unsigned char _broadcastInst;

        size_t _lastMapRefresh;
        size_t _lastWaitMessage;
        size_t _lastJoin;
        size_t _lastMissileUpdate;
        size_t _lastPlayerUpdate;
        size_t _lastMonsterSpawn;
        size_t _lastGameOver;

        void refresh();
        void update();
        void startGame();
        void addMonster(IEntity::Type type, int x, int y);
        void checkCollisionPlayer();
        void checkCollisionMonsters();

    public:
        Room(u_int id, std::shared_ptr<Client> client, bool privateRoom = false);
        ~Room();
        Room(const Room &room) = delete;
        Room(Room &&room) = delete;
        Room &operator=(const Room &room) = delete;
        Room &operator=(Room &&room) = delete;
        u_int getId() const;
        unsigned int getNbPlayer() const;
        unsigned int getProgress() const;
        unsigned int getMaxPlayer() const;
        void addPlayer(std::shared_ptr<Client> client);
        void movePlayer(std::shared_ptr<Client> client, char move, char nbr);
        bool isClientInRoom(std::shared_ptr<Client> client);
        Player &getPlayer(std::shared_ptr<Client> client);
        void sendToAll(const Stream &stream);
        void sendBroadcast();
        Stream &getBroadcastStream();
        void setInstBroadcast(unsigned char inst);
        u_int &getMissilesIds();

};

#endif
