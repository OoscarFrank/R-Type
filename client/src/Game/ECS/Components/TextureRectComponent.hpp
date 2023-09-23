#pragma once

#include <SFML/Graphics.hpp>

namespace ECS{
    namespace components {
        class TextureRectComponent {
            public:
                TextureRectComponent(int left, int top, int width, int height) {
                    this->_textureRect = sf::IntRect(left, top, width, height);
                }

                sf::IntRect &getTextureRect() { return _textureRect; }
                const sf::IntRect &getTextureRect() const { return _textureRect; }

            private:
                sf::IntRect _textureRect;
        };
    }
}
