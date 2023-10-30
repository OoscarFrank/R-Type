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

                        for (const auto &entity : ecs.get_entity_sprite_order()) {
                            auto &sprite = spriteComponents[entity.second];
                            if (sprite && ecs.isEntityEnabled(entity.second) == true) {
                                try {
                                    if (ecs.hasComponent<components::PositionComponent>(entity.second)) {
                                        auto &position = ecs.getComponent<components::PositionComponent>(entity.second);
                                        sprite->setPosition({position.getX(), position.getY()});
                                    }
                                    if (ecs.hasComponent<components::TextureRectComponent>(entity.second)) {
                                        auto &textureRect = ecs.getComponent<components::TextureRectComponent>(entity.second);
                                            sprite->setTextureRect(textureRect.getTextureRect());
                                    }
                                } catch (std::exception &e) {
                                    std::cerr << e.what() << std::endl;
                                }
                                window.draw(sprite->getSprite());
                            }
                        }

                        auto &textComponents = ecs.get_components<components::TextComponent>();

                        for (size_t i = 0; i < textComponents.size() ; ++i) {
                            auto &text = textComponents[i];
                            if (text) {
                                try {
                                    if (ecs.hasComponent<components::PositionComponent>(i)) {
                                        auto &position = ecs.getComponent<components::PositionComponent>(i);
                                        text->setPosition({position.getX(), position.getY()});
                                    }
                                } catch (std::exception &e) {
                                    std::cerr << e.what() << std::endl;
                                }
                                window.draw(text->getText());
                            }
                        }


                        auto &rectangleShapeComponents = ecs.get_components<components::RectangleShapeComponent>();

                        for (size_t i = 0; i < rectangleShapeComponents.size() ; ++i) {
                            auto &rectangleShape = rectangleShapeComponents[i];
                            if (rectangleShape) {
                                try {
                                    if (ecs.hasComponent<components::PositionComponent>(i)) {
                                        auto &position = ecs.getComponent<components::PositionComponent>(i);
                                        rectangleShape->setPosition({position.getX(), position.getY()});
                                    }
                                } catch (std::exception &e) {
                                    std::cerr << e.what() << std::endl;
                                }
                                window.draw(rectangleShape->getRectangleShape());
                            }
                        }

                    } catch (std::exception &e) {
                        std::cerr << e.what() << std::endl;
                    }
                }
        };
    }
}
