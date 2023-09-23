#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <initializer_list>
#include <vector>

namespace ECS {
    namespace components {
        class ControllableComponent {
            public:
                ControllableComponent(std::initializer_list<sf::Keyboard::Key> controls): _controls(controls) {
                }

                const std::vector<sf::Keyboard::Key> &getControls() const {
                    return _controls;
                }

            private:
                std::vector<sf::Keyboard::Key> _controls;
        };
    }
}
