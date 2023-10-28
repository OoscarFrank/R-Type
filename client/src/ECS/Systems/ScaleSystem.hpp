#pragma once

#include "../Registry.hpp"
#include "../Components/Components.hpp"

namespace ECS {
    namespace systems {
        class ScaleSystem {
            public:
            /**
             * @brief Update the scale of each entity with a scale and sprite component
             *
             * @param ecs
             */
                void update(Registry &ecs) {
                    try {
                        auto &scaleComponents = ecs.get_components<components::ScaleComponent>();
                        auto &spriteComponents = ecs.get_components<components::SpriteComponent>();
                        auto &textComponents = ecs.get_components<components::TextComponent>();
                        auto &rectComponents = ecs.get_components<components::RectangleShapeComponent>();

                        for (size_t i = 0; i < spriteComponents.size() && i < scaleComponents.size(); ++i) {
                            auto &scale = scaleComponents[i];
                            auto &sprite = spriteComponents[i];

                            if (sprite && scale) {
                                sprite->setScale(scale->getScaleX(), scale->getScaleY());
                            }
                        }

                        for (size_t i = 0; i < textComponents.size() && i < scaleComponents.size(); ++i) {
                            auto &scale = scaleComponents[i];
                            auto &text = textComponents[i];

                            if (text && scale) {
                                text->setScale(scale->getScaleX(), scale->getScaleY());
                            }
                        }

                        for (size_t i = 0; i < rectComponents.size() && i < scaleComponents.size(); ++i) {
                            auto &scale = scaleComponents[i];
                            auto &rect = rectComponents[i];

                            if (rect && scale) {
                                rect->setScale(scale->getScaleX(), scale->getScaleY());
                            }
                        }

                    } catch (std::exception &e) {
                        std::cerr << e.what() << std::endl;
                    }
                }
        };
    }
}
