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

                const sf::Vector2f &getSize() const { return _bar.getSize(); }

                const sf::Color &getColor() const { return _bar.getFillColor(); }

                const sf::Vector2f &getPosition() const { return _bar.getPosition(); }

                sf::RectangleShape &getRectangleShape() { return _bar; }

            private:
                sf::RectangleShape _bar;
        };
    }
}
