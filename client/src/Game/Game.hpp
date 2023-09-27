#pragma once

#include "../Core.hpp"
#include "./EventManager.hpp"
#include "./ECS/Loader/Loader.hpp"
#include "../Utils.hpp"
#include "./ECS/Registry.hpp"
#include "../Network/Network.hpp"
#include <vector>
#include "./ECS/Systems/Systems.hpp"

namespace game {
    class Game: public EventManager {
        public:
            Game();
            ~Game();
            int MainLoop();
            void update();
            entity_t getPlayerEntityFromId(unsigned char id);
            entity_t getMissileEntityFromId(unsigned char id);
            void sendMoveToServer();

        private:
            sf::Vector2u _screenSize;
            sf::RenderWindow _window;
            long _lastTime;
            Loader _manager;
            ECS::Registry ecs;
            Network _net;
            float _resMult;

            unsigned short _roomId;
            unsigned char _playerId;
            entity_t _playerEntity;

            unsigned int _startTimeLeft;
            unsigned char _started;
            std::vector<std::pair<size_t, entity_t>> _players;
            std::vector<std::pair<size_t, entity_t>> _missiles;
            std::vector<ECS::systems::MovableSystem::EntityPos> _entityPositions;
            std::vector<ECS::systems::ControllableSystem::EntityMove> _entityMoves;
    };
}
