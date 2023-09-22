#pragma once

#include "../Core.hpp"
#include "./EventManager.hpp"
#include "./Loader/Loader.hpp"
#include "../Utils.hpp"

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
    };
}
