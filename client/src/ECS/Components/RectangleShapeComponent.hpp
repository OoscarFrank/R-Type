#pragma once
#include <SFML/Graphics.hpp>

namespace ECS {
    namespace components {
        class RectangleShapeComponent {
            public:
            /**
             * @brief Construct a new Rectangle Shape Component object
             * 
             * @param size 
             * @param color 
             * @param pos 
             */
                RectangleShapeComponent(const sf::Vector2f &size, const sf::Color &color, const sf::Vector2f &pos) {
                    _bar.setSize(size);
                    _bar.setFillColor(color);
                    _bar.setPosition(pos);
                }
            /**
             * @brief Construct a new Rectangle Shape Component object
             * 
             * @param texture 
             * @param rect 
             * @param pos 
             */
                RectangleShapeComponent(const std::shared_ptr<sf::Texture> &texture, const sf::IntRect &rect, const sf::Vector2f &pos) {
                    _bar.setSize(sf::Vector2f(rect.width, rect.height));
                    _bar.setTexture(texture.get());
                    _bar.setTextureRect(rect);
                    _bar.setPosition(pos);
                }
            /**
             * @brief Set the Texture object
             * 
             * @param texture 
             */
                void setTexture(const std::shared_ptr<sf::Texture> &texture) {
                    _bar.setTexture(texture.get());
                }
            /**
             * @brief Set the Size object
             * 
             * @param size 
             */
                void setSize(const sf::Vector2f &size) { _bar.setSize(size); }
            /**
             * @brief Set the Color object
             * 
             * @param color 
             */
                void setColor(const sf::Color &color) { _bar.setFillColor(color); }
            /**
             * @brief Set the Position object
             * 
             * @param pos 
             */
                void setPosition(const sf::Vector2f &pos) { _bar.setPosition(pos); }
            /**
             * @brief Set the Texture Rect object
             * 
             * @param rect 
             */
                void setTextureRect(const sf::IntRect &rect) { _bar.setTextureRect(rect); }
            /**
             * @brief Get the Texture Rect object
             * 
             * @return const sf::IntRect& 
             */
                const sf::IntRect &getTextureRect() const { return _bar.getTextureRect(); }
            /**
             * @brief Get the Size object
             * 
             * @return const sf::Vector2f& 
             */
                const sf::Vector2f &getSize() const { return _bar.getSize(); }
            /**
             * @brief Get the Color object
             * 
             * @return const sf::Color& 
             */
                const sf::Color &getColor() const { return _bar.getFillColor(); }
            /**
             * @brief Get the Position object
             * 
             * @return const sf::Vector2f& 
             */
                const sf::Vector2f &getPosition() const { return _bar.getPosition(); }
            /**
             * @brief Get the Rectangle Shape object
             * 
             * @return sf::RectangleShape& 
             */
                sf::RectangleShape &getRectangleShape() { return _bar; }
            /**
             * @brief Set the Scale object
             * 
             * @param scaleX 
             * @param scaleY 
             */
                void setScale(const float &scaleX, const float &scaleY) {
                    if (_bar.getScale().x != scaleX || _bar.getScale().y != scaleY) {
                        _bar.setScale(scaleX, scaleY);
                    }
                }
            private:
            /**
             * @brief Store the rectangle shape
             * 
             */
                sf::RectangleShape _bar;
        };
    }
}
