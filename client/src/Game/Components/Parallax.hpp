#pragma once

#include "../../Error/MyError.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace game {
    namespace components {
        class Parallax {
            public:
                Parallax(const std::string &backgroundPath, const sf::Vector2f &firstPosition, float speed);
                ~Parallax();

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
