#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "../Utils.hpp"

namespace game {
    class EventManager {
        public:
            EventManager();
            ~EventManager();
            enum keys {
                Escape
            };
            void EventLoop(sf::RenderWindow &window);
            bool isKeyPressed(EventManager::keys key);
        private:
            sf::Event _event;
            long long _timeElapsed;
    };
}
