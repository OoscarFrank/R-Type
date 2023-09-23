#pragma once

#include <SFML/Graphics.hpp>

namespace ECS{
    namespace components {
        class SpriteComponent {
            public:
                SpriteComponent(const sf::Texture &texture) {
                    this->_sprite.setTexture(texture);
                }

                void setTexture(const sf::Texture &texture) { this->_sprite.setTexture(texture); }
                void setPosition(const sf::Vector2f &position) { this->_sprite.setPosition(position); }
                void setTextureRect(const sf::IntRect &rect) { this->_sprite.setTextureRect(rect); };

                sf::Sprite &getSprite() { return _sprite; }
                const sf::Sprite &getSprite() const { return _sprite; }

            private:
                sf::Sprite _sprite;
        };
    }
}
