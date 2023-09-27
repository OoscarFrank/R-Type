#pragma once

#include "../Core.hpp"
#include "./EventManager.hpp"
#include "./ECS/Loader/Loader.hpp"
#include "../Utils.hpp"
#include "./ECS/Registry.hpp"
#include "../Network/Network.hpp"

namespace game {
    class Game: public EventManager {
        public:
            Game();
            ~Game();
            int MainLoop();
            void update();

        private:
            sf::Vector2u _screenSize;
            sf::RenderWindow _window;
            long _lastTime;
            Loader _manager;
            ECS::Registry ecs;
            Network _net;
    };
}
