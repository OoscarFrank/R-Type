#pragma once
#include <SFML/Graphics.hpp>

namespace ECS {
    namespace components {
        class RectangleShapeComponent {
            public:
                RectangleShapeComponent(const sf::Vector2f &size, const sf::Color &color, const sf::Vector2f &pos) {
                    _bar.setSize(size);
                    _bar.setFillColor(color);
                    _bar.setPosition(pos);
                }

                RectangleShapeComponent(const std::shared_ptr<sf::Texture> &texture, const sf::IntRect &rect, const sf::Vector2f &pos) {
                    _bar.setSize(sf::Vector2f(rect.width, rect.height));
                    _bar.setTexture(texture.get());
                    _bar.setTextureRect(rect);
                    _bar.setPosition(pos);
                }

                void setTexture(const std::shared_ptr<sf::Texture> &texture) {
                    _bar.setTexture(texture.get());
                }

                void setSize(const sf::Vector2f &size) { _bar.setSize(size); }

                void setColor(const sf::Color &color) { _bar.setFillColor(color); }

                void setPosition(const sf::Vector2f &pos) { _bar.setPosition(pos); }

                void setTextureRect(const sf::IntRect &rect) { _bar.setTextureRect(rect); }

                const sf::IntRect &getTextureRect() const { return _bar.getTextureRect(); }

                const sf::Vector2f &getSize() const { return _bar.getSize(); }

                const sf::Color &getColor() const { return _bar.getFillColor(); }

                const sf::Vector2f &getPosition() const { return _bar.getPosition(); }

                sf::RectangleShape &getRectangleShape() { return _bar; }

                void setScale(const float &scaleX, const float &scaleY) {
                    if (_bar.getScale().x != scaleX || _bar.getScale().y != scaleY) {
                        _bar.setScale(scaleX, scaleY);
                    }
                }

            private:
                sf::RectangleShape _bar;
        };
    }
}
