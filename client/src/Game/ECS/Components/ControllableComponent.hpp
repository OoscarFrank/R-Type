#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <initializer_list>
#include <vector>

namespace ECS {
    namespace components {
        class ControllableComponent {
            public:
            /**
             * @brief Construct a new Controllable Component object
             * 
             * @param controls 
             */
                ControllableComponent(std::initializer_list<sf::Keyboard::Key> controls): _controls(controls) {
                }
            /**
             * @brief Get the Controls object
             * 
             * @return const std::vector<sf::Keyboard::Key>& 
             */
                const std::vector<sf::Keyboard::Key> &getControls() const {
                    return _controls;
                }

            private:
                std::vector<sf::Keyboard::Key> _controls;
        };
    }
}
