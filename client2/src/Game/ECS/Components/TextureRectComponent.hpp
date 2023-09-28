#pragma once

#include <SFML/Graphics.hpp>

namespace ECS {
    namespace components {
        class TextureRectComponent {
        public:
            TextureRectComponent(int left, int top, int totalWidth, int height, int numFrames, float frameDelay):
                _textureRect(left, top, totalWidth / numFrames, height),
                _frameWidth(totalWidth / numFrames),
                _numFrames(numFrames),
                _frameDelay(frameDelay),
                _timeSinceLastFrameChange(0) {
            }

            sf::IntRect &getTextureRect() { return _textureRect; }
            const sf::IntRect &getTextureRect() const { return _textureRect; }

            sf::IntRect _textureRect;
            int _frameWidth;
            int _numFrames;
            float _frameDelay;
            float _timeSinceLastFrameChange = 0.0f;

        };
    }
}
