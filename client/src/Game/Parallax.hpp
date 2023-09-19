#pragma once

#include "../Error/MyError.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace client
{
    class Parallax {
        public:
            Parallax(const std::string &backgroundPath, sf::Vector2f firstPosition, float speed);
            ~Parallax();

            void update(float deltaTime);
            void draw(sf::RenderWindow &window);

        private:
            sf::Sprite _background;
            sf::Texture _texture;
            sf::Vector2f _position;
            float _speed;
    };
}
