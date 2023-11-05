#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "ArmedEntity.hpp"
#include "../ForcePod.hpp"

class Player: public ArmedEntity {
    public:
        /**
         * @brief Construct a new Player object
         *
         * @param room The room the player is in
         * @param client The client the player is linked to
         * @param id The player's id
         * @param x The player's x position
         * @param y The player's y position
         */
        Player(Room &room, std::shared_ptr<Client> client, u_int id, short x, short y);
        /**
         * @brief Construct a new Player object
         *
         * @param room The room the player is in
         * @param client The client the player is linked to
         * @param id The player's id
         * @param pos The player's position
         */
        Player(Room &room, std::shared_ptr<Client> client, u_int id, const std::pair<short, short> &pos);
        virtual ~Player();

        /**
         * @brief Refresh the player
         * This method currently only refresh the missiles of the player by calling refreshMissiles()
         *
         */
        virtual void refresh();
        /**
         * @brief Checks if any missile of the player collide with another entity (this method override the one from IEntity)
         *
         * @param other The other entity to check collision with
         * @return true If the player collides with the other entity
         * @return false If the player doesn't collide with the other entity
         */
        virtual bool collide(const IEntity &other) override;
        /**
         * @brief Move the player
         * This method checks if the player is out of the screen and if it is, it doesn't move it.
         *
         * @param dx Delta x
         * @param dy Delta y
         */
        virtual void move(short dx, short dy);
        /**
         * @brief Send the player's position to all the clients in the room
         *
         */
        virtual void sendPos();
        /**
         * @brief Fire a missile
         * This method checks the time between the last missile fired and the current time and if it's less than PLAYER_FIRE_TIME, it doesn't fire a missile.
         *
         */
        virtual void fireMissile();
        virtual int getDamage();
        /**
         * @brief Set the player's life
         *
         * @param life The new life to get
         */
        virtual void setLife(int life);
        virtual void kill() override;

        /**
         * @brief Get the last move time
         * This method is used by the room to not move the player too fast
         *
         * @return const std::chrono::system_clock::time_point&
         */
        const std::chrono::system_clock::time_point &lastMoveTime() const;
        /**
         * @brief Set the last move time
         * This method is used by the room to not move the player too fast
         *
         * @param lastMoveTime The new last move time to set
         */
        void setLastMoveTime(const std::chrono::system_clock::time_point &lastMoveTime);
        /**
         * @brief Get the player's score
         *
         * @return int
         */
        int score() const;
        /**
         * @brief Set the player's score
         *
         * @param score The new score to get
         */
        void setScore(int score);
        /**
         * @brief Get the player's client
         *
         * @return std::shared_ptr<Client>
         */
        std::shared_ptr<Client> client() const;

        /**
         * @brief Get the pod missile level
         *
         * @return unsigned char
         */
        unsigned char podMissileLvl() const;

        /**
         * @brief Set the pod missile level
         *
         * @param podMissileLvl The new pod missile level to set
         */
        void setPodMissileLvl(unsigned char podMissileLvl);

        /**
         * @brief Get the force pod
         *
         * @return ForcePod&
         */
        ForcePod &forcePod();
    private:
        unsigned char _podMissileLvl = 1;
        int _score;
        std::shared_ptr<Client> _client;
        ForcePod _forcePod;
};

#endif
