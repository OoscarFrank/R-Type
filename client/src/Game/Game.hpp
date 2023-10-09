#pragma once

#include "../Core.hpp"
#include "./EventManager.hpp"
#include "./ECS/Loader/Loader.hpp"
#include "../Utils.hpp"
#include "./ECS/Registry.hpp"
#include "../Network/Network.hpp"
#include <vector>
#include "./ECS/Systems/Systems.hpp"
#include "Factory.hpp"

namespace game {
    class Game: public EventManager {
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
        /**
         * @brief Get the Player Entity From Id object
         *
         * @param id
         * @return entity_t
         */
            entity_t getPlayerEntityFromId(unsigned int id);
        /**
         * @brief Get the Missile Entity From Id object
         *
         * @param id
         * @return entity_t
         */
            entity_t getMissileEntityFromId(unsigned int id);
        /**
         * @brief Get the Ennemi Entity From Id object
         *
         * @param id
         * @return entity_t
         */
            entity_t getEnnemiEntityFromId(unsigned int id);
            void sendMoveToServer();

        private:
            sf::Vector2u _screenSize;
            sf::RenderWindow _window;
            long _lastTime;
            Loader _manager;
            ECS::Registry ecs;
            Factory _factory;
            Network _net;
            float _resMult;

            unsigned int _roomId;
            unsigned int _playerId;
            entity_t _playerEntity;
            bool _gameOver;

            unsigned int _startTimeLeft;
            unsigned char _started;

            unsigned char moveMemory;

            std::vector<std::pair<size_t, entity_t>> _players;
            std::vector<std::pair<size_t, entity_t>> _missiles;
            std::vector<std::pair<size_t, entity_t>> _ennemies;
            std::vector<ECS::systems::MovableSystem::EntityPos> _entityPositions;
            std::vector<ECS::systems::ControllableSystem::EntityMove> _entityMoves;
    };
}
