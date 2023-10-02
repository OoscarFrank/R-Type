#pragma once

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
        bool _started;
        std::thread _thread;
        std::mutex _playersMutex;
        std::vector<std::unique_ptr<Player>> _players;
        std::vector<std::unique_ptr<IEntity>> _monsters;
        u_int _id;
        unsigned int _maxPlayer;
        unsigned int _progress;
        u_int _playersIds;
        bool _private;
        void refresh();
        void update();

        size_t _missilesIds;
        u_int _monstersIds;

        Stream _broadcastStream;
        unsigned char _broadcastInst;

        size_t _lastMapRefresh;
        size_t _lastWaitMessage;
        size_t _lastJoin;
        size_t _lastMissileUpdate;
        size_t _lastPlayerUpdate;
        size_t _lastMonsterSpawn;

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
        size_t &getMissilesIds();

    private:
        void startGame();
        void addMonster(IEntity::Type type, int x, int y);
        void checkCollisionPlayer();
        void checkCollisionMonsters();
};
