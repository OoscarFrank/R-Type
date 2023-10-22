#ifndef ROOM_HPP
#define ROOM_HPP

#include <iostream>
#include <vector>
#include <thread>
#include "Entities/Entity.hpp"
#include "Entities/Player.hpp"
#include "Entities/Monsters/Little.hpp"
#include "Entities/Monsters/Zigzager.hpp"
#include "Entities/Monsters/Follower.hpp"
#include "../Client.hpp"
#include "Levels.hpp"

class Room
{
    public:
        enum State {
            WAIT = 0,
            RUN = 1,
            END = 2,
            STOPPED = 3
        };

    private:
        State _state;
        std::thread _thread;
        std::mutex _playersMutex;
        std::vector<std::unique_ptr<Player>> _players;
        std::vector<std::unique_ptr<Monster>> _monsters;
        u_int _id;
        unsigned int _maxPlayer;
        unsigned int _progress;
        u_int _playersIds;
        bool _private;
        Levels _levels;

        u_int _missilesIds;
        u_int _monstersIds;

        size_t _lastMapRefresh;
        size_t _lastWaitMessage;
        size_t _lastJoin;
        size_t _lastMissileUpdate;
        size_t _lastPlayerUpdate;
        size_t _lastGameOver;

        void refresh();
        void update();
        void startGame();
        void checkCollisionPlayer();
        void checkCollisionMonsters();

    public:
        /**
         * @brief Construct a new Room object
         *
         * @param id The id of the room
         * @param client The client that created the room
         * @param privateRoom If the room is private or not (default: false)
         */
        Room(u_int id, std::shared_ptr<Client> client, bool privateRoom = false);
        ~Room();
        Room(const Room &room) = delete;
        Room(Room &&room) = delete;
        Room &operator=(const Room &room) = delete;
        Room &operator=(Room &&room) = delete;
        /**
         * @brief Get the state of the room (WAIT | RUN | END | STOPPED)
         *
         * @return State
         */
        State getState() const;
        size_t getCurrentLevel() const;
        /**
         * @brief Get the id of the room
         *
         * @return u_int
         */
        u_int getId() const;
        /**
         * @brief Get the number of players in the room
         *
         * @return unsigned int
         */
        unsigned int getNbPlayer() const;
        /**
         * @brief Get the progress of the game
         *
         * @return unsigned int
         */
        unsigned int getProgress() const;
        /**
         * @brief Get max number of players allowed in the room
         *
         * @return unsigned int
         */
        unsigned int getMaxPlayer() const;
        /**
         * @brief Add a new player to the room
         *
         * @param client The client that wants to join the room
         */
        void addPlayer(std::shared_ptr<Client> client);
        /**
         * @brief Move a player in the room
         *
         * @param client The client that wants to move
         * @param move The move to make (PLAYER_MOVE_UP | PLAYER_MOVE_DOWN | PLAYER_MOVE_LEFT | PLAYER_MOVE_RIGHT) you can combine them with the | binary operator
         * @param nbr The number of move to make (default: 1)
         */
        void movePlayer(std::shared_ptr<Client> client, char move, char nbr = 1);
        /**
         * @brief Checks if a client is in the room
         *
         * @param client The client to check
         * @return true If the client is in the room
         * @return false If the client is not in the room
         */
        bool isClientInRoom(std::shared_ptr<Client> client);
        /**
         * @brief Get the player associated with a client
         *
         * @param client The client to find
         * @return Player&
         */
        Player &getPlayer(std::shared_ptr<Client> client);
        /**
         * @brief Send a packet to all players in the room
         *
         * @param stream The packet to send
         */
        void sendToAll(const Stream &stream);
        /**
         * @brief Get the id of the next missile to create
         *
         * @return u_int&
         */
        u_int &getMissilesIds();
        void addMonster(IEntity::Type type, int x, int y);
        std::pair<short, short> getNearestPlayerPos(const IEntity &entity);
};

#endif
