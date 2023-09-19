#pragma once

#include "../../Error/MyError.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace game {
    namespace components {
        class Parallax {
            public:
                Parallax(const sf::Vector2f &firstPosition, float speed);
                ~Parallax();

                void setTexture(const sf::Texture &texture);
                void update(float deltaTime);
                void draw(sf::RenderWindow &window) const;

            private:
                sf::Sprite _background;
                sf::Texture _texture;
                sf::Vector2f _position;
                float _speed;
        };
    }
}
