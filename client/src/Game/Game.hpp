#pragma once

#include "../Core.hpp"
#include "./Loader/Loader.hpp"
#include "../Utils.hpp"
#include "./ECS/Registry.hpp"
#include "../Network/Network.hpp"
#include "EntitiesFactory.hpp"
#include "EntityManager.hpp"

namespace game {
    class Game: public EntityManager {
        public:
        /**
         * @brief Construct a new Game object
         *
         */
            Game();
        /**
         * @brief Destroy the Game object
         *
         */
            ~Game();
        /**
         * @brief function who's looping on the window and call everythings
         *
         * @return int
         */
            int MainLoop();
        /**
         * @brief update data for entities
         *
         */
            void update();

            void sendMoveToServer();

        private:
            sf::Vector2u _screenSize;
            sf::RenderWindow _window;

            Loader _manager;
            ECS::Registry ecs;
            Factory _factory;
            Network _net;

            long _lastTime;
            float _resMult;

            unsigned int _roomId;
            unsigned int _playerId;
            bool _gameOver;

            unsigned int _startTimeLeft;
            unsigned char _started;

            int eventMemory;

    };
}
