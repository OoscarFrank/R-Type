#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "../Utils.hpp"
#include "../Network/Network.hpp"

namespace game {
    class EventManager {
        public:
            EventManager();
            ~EventManager();
            enum keys {
                Escape
            };
            void EventLoop(sf::RenderWindow &window, Network &_net);
            bool isKeyPressed(EventManager::keys key);
        private:
            sf::Event _event;
            long long _timeElapsed;
    };
}
