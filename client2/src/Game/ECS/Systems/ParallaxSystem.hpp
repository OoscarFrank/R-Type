#pragma once

#include "../Registry.hpp"
#include "../Components/Components.hpp"

namespace ECS {
    namespace systems {
        class ParallaxSystem {
        public:
            void update(Registry &ecs, float deltaTime) {
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

                        position->setX(newX);
                        // position->setY(newY);
                    }
                }
            }
        };
    }
}
