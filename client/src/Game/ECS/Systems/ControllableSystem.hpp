#include <SFML/Window/Keyboard.hpp>
#include "../Registry.hpp"
#include "../Components/Components.hpp"

namespace ECS {
    namespace systems {
        class ControllableSystem {
        public:
            ControllableSystem() = default;

            void update(Registry &ecs, float deltaTime) {
                auto &ControllableComponents = ecs.get_components<components::ControllableComponent>();
                auto &VelocityComponents = ecs.get_components<components::VelocityComponent>();

                for (size_t i = 0; i < ControllableComponents.size(); ++i) {
                    auto &ControllableComponent = ControllableComponents[i];

                    if (ControllableComponent) {
                        auto entity = ecs.entity_from_index(i);

                        float velocityChangeX = VelocityComponents[entity]->getInitialDX() * deltaTime;
                        float velocityChangeY = VelocityComponents[entity]->getInitialDY() * deltaTime;

                        for (auto key : ControllableComponent->getControls()) {
                            if (sf::Keyboard::isKeyPressed(key)) {
                                switch (key) {
                                    case sf::Keyboard::Up:
                                        VelocityComponents[entity]->setDY(-velocityChangeY);
                                        break;
                                    case sf::Keyboard::Down:
                                        VelocityComponents[entity]->setDY(velocityChangeY);
                                        break;
                                    case sf::Keyboard::Left:
                                        VelocityComponents[entity]->setDX(-velocityChangeX);
                                        break;
                                    case sf::Keyboard::Right:
                                        VelocityComponents[entity]->setDX(velocityChangeX);
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                    }
                }
            }

        };
    }
}
