#pragma once

#include <SFML/Graphics.hpp>

namespace ECS {
    namespace components {
        class TextureRectComponent {
        public:
        /**
         * @brief Construct a new Texture Rect Component object
         * 
         * @param left 
         * @param top 
         * @param totalWidth 
         * @param height 
         * @param numFrames 
         * @param frameDelay 
         */
            TextureRectComponent(int left, int top, int totalWidth, int height, int numFrames, float frameDelay):
                _textureRect(left, top, totalWidth / numFrames, height),
                _frameWidth(totalWidth / numFrames),
                _numFrames(numFrames),
                _frameDelay(frameDelay),
                _timeSinceLastFrameChange(0) {
            }
        /**
         * @brief Get the Texture Rect object
         * 
         * @return sf::IntRect& 
         */
            sf::IntRect &getTextureRect() { return _textureRect; }
        /**
         * @brief Get the Texture Rect object
         * 
         * @return const sf::IntRect& 
         */
            const sf::IntRect &getTextureRect() const { return _textureRect; }
            void setFrameOnTexture(int frame) {
                if (frame > _numFrames)
                    frame = 0;
                _textureRect.left = frame * _frameWidth;
            }

            sf::IntRect _textureRect;
            int _frameWidth;
            int _numFrames;
            float _frameDelay;
            float _timeSinceLastFrameChange = 0.0f;

        };
    }
}
