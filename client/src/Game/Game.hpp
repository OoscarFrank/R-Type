#pragma once

#include "../Core.hpp"
#include "./EventManager.hpp"
#include "./ECS/Loader/Loader.hpp"
#include "../Utils.hpp"
#include "./ECS/Registry.hpp"

namespace game {
    class Game: public EventManager {
        public:
            Game();
            ~Game();
            int MainLoop();

        private:
            sf::Vector2u _screenSize;
            sf::RenderWindow _window;
            long _lastTime;
            Loader _manager;
            ECS::Registry ecs;
    };
}
