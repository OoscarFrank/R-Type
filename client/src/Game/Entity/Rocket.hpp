#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "../../Error/MyError.hpp"
#include "../Loader/Loader.hpp"

namespace game {
    namespace entity {
        class Rocket {
            public:
                Rocket(const sf::Vector2f &firstPosition, const loader::Loader &loader);
                ~Rocket();

                void draw(sf::RenderWindow &window) const;
                void setTexture(const sf::Texture &texture);
                void setPosition(const sf::Vector2f &position);
                const sf::Vector2f &getPosition();
                void update(float deltaTime);
                sf::Sprite _sprite;

            private:
                sf::Vector2f _position;
                sf::Texture _texture;
                float _speed;
        };
    }
}