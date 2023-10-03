#pragma once

#include "../Registry.hpp"
#include "../Components/Components.hpp"

namespace ECS {
    namespace systems {
        class DrawSystem {
            public:
            /**
             * @brief Update the sprite of each entity with a sprite component
             *
             * @param ecs
             * @param window
             */
                void update(Registry &ecs, sf::RenderWindow &window) {
                    try {
                        auto &spriteComponents = ecs.get_components<components::SpriteComponent>();

                        for (size_t i = 0; i < spriteComponents.size(); ++i) {
                            auto &sprite = spriteComponents[i];

                            if (sprite) {
                                try {
                                    if (ecs.hasComponent<components::PositionComponent>(i)) {
                                        auto &position = ecs.getComponent<components::PositionComponent>(i);
                                        sprite->setPosition({position.getX(), position.getY()});
                                    }

                                    if (ecs.hasComponent<components::TextureRectComponent>(i)) {
                                        auto &textureRect = ecs.getComponent<components::TextureRectComponent>(i);
                                        sprite->setTextureRect(textureRect.getTextureRect());
                                    }
                                } catch (std::exception &e) {
                                    std::cerr << e.what() << std::endl;
                                }

                                window.draw(sprite->getSprite());
                            }
                        }
                    } catch (std::exception &e) {
                        std::cerr << e.what() << std::endl;
                    }
                }
        };
    }
}
