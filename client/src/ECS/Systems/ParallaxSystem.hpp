#pragma once

#include "../Registry.hpp"
#include "../Components/Components.hpp"

namespace ECS {
    namespace systems {
        class ParallaxSystem {
        public:
        /**
         * @brief Update the position of each entity with a position and a parallax component
         * 
         * @param ecs 
         * @param deltaTime 
         */
            void update(Registry &ecs, float deltaTime, sf::Vector2u offset = {0, 0}) {
                try {
                    auto &parallaxComponents = ecs.get_components<components::ParallaxComponent>();
                    auto &positionComponents = ecs.get_components<components::PositionComponent>();

                    for (size_t i = 0; i < parallaxComponents.size() && i < positionComponents.size(); ++i) {
                        auto &parallax = parallaxComponents[i];
                        auto &position = positionComponents[i];

                        if (parallax && position) {
                            float scrollSpeed = parallax->getScrollSpeed();
                            float newX = position->getX() + deltaTime * scrollSpeed;
                            // float newY = position->getY() + deltaTime * scrollSpeed;

                            float backgroundWidth = parallax->getBackgroundWidth() / 2;
                            if (newX < -backgroundWidth) {
                                newX += backgroundWidth;
                            }

                            position->setX(newX + offset.x);
                            position->setY(offset.y);
                            // position->setY(newY);
                        }
                    }
                } catch (std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        };
    }
}
