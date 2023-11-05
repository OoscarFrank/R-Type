#pragma once
#include <iostream>
#include <vector>
#include "Entities/Bomb.hpp"
#include "Entities/Laser.hpp"
#include <memory>
#include <mutex>
#include "Entities/Ray.hpp"
#include "Entities/Laser.hpp"
#include "Entities/Bomb.hpp"

class Room;
class Player;

class ForcePod {
    public:
        /**
         * @brief Construct a new Force Pod object
         *
         * @param room The room the entity is in
         * @param player The player the entity is attached to
         */
        ForcePod(Room &room, Player &player);

        /**
         * @brief Destroy the Force Pod object
         *
         */
        ~ForcePod() = default;

        /**
         * @brief Refresh all forcepod weapons
         *
         */
        void refresh();

        /**
         * @brief Toggle the direction of the forcepod
         *
         */
        void toggleFront();

        /**
         * @brief Shoot a bomb
         *
         */
        void shootBomb();

        /**
         * @brief Shoot a laser
         *
         */
        void shootLaser();

        /**
         * @brief Shoot a ray
         *
         */
        void shootRay();

        /**
         * @brief Check if an entity collding with a bomb
         * 
         * @param other The other entity
         * 
         * @return true If the bomb is colliding
         */
        void bombCollide(IEntity &other);

        /**
         * @brief Check if an entity collding with a laser
         * 
         * @param other The other entity
         * 
         * @return true If the laser is colliding
         */
        void laserCollide(IEntity &other);

        /**
         * @brief Check if an entity collding with a ray
         * 
         * @param other The other entity
         * 
         * @return true If the ray is colliding
         */
        void rayCollide(IEntity &other);

        /**
         * @brief change the lvl of the forcepod
         * 
         * @param lvl The new lvl
         */
        void setLvl(u_char lvl);

        /**
         * @brief Get the lvl of the forcepod
         * 
         * @return The lvl of the forcepod
         */
        u_char getLvl() const;

        /**
         * @brief Get the last time point of the laser hit refresh
         * 
         */
        std::chrono::system_clock::time_point _lastLaserHit;

        /**
         * @brief Get the last time point of the ray hit refresh
         * 
         */
        std::chrono::system_clock::time_point _lastRayHit;
    private:
        bool _isFront;
        std::chrono::system_clock::time_point _lastBomb;
        std::chrono::system_clock::time_point _lastRay;
        std::chrono::system_clock::time_point _lastLaser;


        u_char lvl = 0;
        std::vector<std::unique_ptr<Bomb>> _bombs;
        std::vector<std::unique_ptr<Laser>> _lasers;
        std::vector<std::unique_ptr<Ray>> _rays;
        Room &_room;
        Player &_player;
};