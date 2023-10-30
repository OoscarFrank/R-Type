#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

namespace ECS{
    namespace components {
        class SpriteComponent {
            public:
            /**
             * @brief Construct a new Sprite Component object
             *
             * @param texture
             */
                SpriteComponent(const std::shared_ptr<sf::Texture> &texture) {
                    this->_sprite.setTexture(*texture);
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

                void setScale(const float &scaleX, const float &scaleY) {
                    if (this->_sprite.getScale().x != scaleX || this->_sprite.getScale().y != scaleY) {
                        this->_sprite.setScale(scaleX, scaleY);
                    }
                }

            /**
             * @brief Set the Texture Rect object
             *
             * @param rect
             */
                void setTextureRect(const sf::IntRect &rect) { this->_sprite.setTextureRect(rect); };

            /**
             * @brief Get the Texture Rect object
             *
             * @return const sf::IntRect&
             */
                const sf::IntRect &getTextureRect() const { return _sprite.getTextureRect(); }
            /**
             *
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
