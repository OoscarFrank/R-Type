#pragma once

#include <SFML/Graphics.hpp>

namespace ECS{
    namespace components {
        class SpriteComponent {
            public:
            /**
             * @brief Construct a new Sprite Component object
             * 
             * @param texture 
             */
                SpriteComponent(const sf::Texture &texture) {
                    this->_sprite.setTexture(texture);
                }
            /**
             * @brief Set the Texture object
             * 
             * @param texture 
             */
                void setTexture(const sf::Texture &texture) { this->_sprite.setTexture(texture); }
            /**
             * @brief Set the Position object
             * 
             * @param position 
             */
                void setPosition(const sf::Vector2f &position) { this->_sprite.setPosition(position); }
            /**
             * @brief Set the Texture Rect object
             * 
             * @param rect 
             */
                void setTextureRect(const sf::IntRect &rect) { this->_sprite.setTextureRect(rect); };
            /**
             * @brief Get the Sprite object
             * 
             * @return sf::Sprite& 
             */
                sf::Sprite &getSprite() { return _sprite; }
            /**
             * @brief Get the Sprite object
             * 
             * @return const sf::Sprite& 
             */
                const sf::Sprite &getSprite() const { return _sprite; }

            private:
                sf::Sprite _sprite;
        };
    }
}
