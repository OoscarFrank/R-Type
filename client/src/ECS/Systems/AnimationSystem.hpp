#pragma once

#include "../Registry.hpp"
#include "../Components/Components.hpp"

namespace ECS {
    namespace systems {
        class AnimationSystem {
            public:
            /**
             * @brief Update animation of each entity with a texture rect component
             * 
             * @param ecs 
             * @param deltaTime 
             */
                void update(Registry &ecs, float deltaTime) {
                    try {
                        auto &textureRectComponents = ecs.get_components<components::TextureRectComponent>();
                        auto &animationComponents = ecs.get_components<components::AnimationComponent>();

                        for (size_t i = 0; i < animationComponents.size() && i < textureRectComponents.size(); ++i) {
                            auto &textureRect = textureRectComponents[i];
                            auto &animation = animationComponents[i];

                            if (animation && textureRect) {
                                textureRect->_timeSinceLastFrameChange += deltaTime;

                                if (textureRect->_timeSinceLastFrameChange >= textureRect->_frameDelay) {
                                    textureRect->_textureRect.left += textureRect->_frameWidth;
                                    if (textureRect->_textureRect.left + textureRect->_frameWidth > textureRect->_frameWidth * textureRect->_numFrames) {
                                        textureRect->_textureRect.left = 0;
                                    }

                                    textureRect->_timeSinceLastFrameChange = 0.0f;
                                }
                            }
                        }
                    } catch (std::exception &e) {
                        std::cerr << e.what() << std::endl;
                    }
                }
        };
    }
}
