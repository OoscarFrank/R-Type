#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "../Utils.hpp"
#include "../Network/Network.hpp"

namespace game {
    class EventManager {
        public:
        /**
         * @brief Construct a new Event Manager object
         * 
         */
            EventManager();
        /**
         * @brief Destroy the Event Manager object
         * 
         */
            ~EventManager();
            enum keys {
                Escape
            };
        /**
         * @brief Handle when the utser press down a key on the keyboard
         * 
         * @param window 
         * @param _net 
         */
            void EventLoop(sf::RenderWindow &window, Network &_net);
            bool isKeyPressed(EventManager::keys key);
        private:
            sf::Event _event;
            long long _timeElapsed;
    };
}
