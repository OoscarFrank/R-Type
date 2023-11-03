#pragma once

#include "../Registry.hpp"
#include "../Components/Components.hpp"

namespace ECS {
    namespace systems {
        class AnimationOneTimeSystem {
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
                        auto &animationOneTimeComponents = ecs.get_components<components::AnimationOneTimeComponent>();

                        for (size_t i = 0; i < animationOneTimeComponents.size() && i < textureRectComponents.size(); ++i) {
                            auto &textureRect = textureRectComponents[i];
                            auto &animation = animationOneTimeComponents[i];

                            if (animation && textureRect) {
                                textureRect->_timeSinceLastFrameChange += deltaTime;

                                if (textureRect->_timeSinceLastFrameChange >= textureRect->_frameDelay) {
                                    if (animation->_frame == textureRect->_numFrames - 1) {
                                        ecs.kill_entity(static_cast<entity_t>(i));
                                        continue;
                                    }
                                    textureRect->_textureRect.left += textureRect->_frameWidth;
                                    if (textureRect->_textureRect.left + textureRect->_frameWidth > textureRect->_frameWidth * textureRect->_numFrames) {
                                        textureRect->_textureRect.left = 0;
                                    }
                                    animation->_frame++;
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
