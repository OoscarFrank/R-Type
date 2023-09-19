#pragma once

#include <SFML/Graphics.hpp>
#include "../../Error/MyError.hpp"

namespace game {
    namespace entity {
        class Player {
            public:
                Player(const std::string &backgroundPath, const sf::Vector2f &firstPosition, float life);
                ~Player();

                void move(const sf::Vector2f &position);
                void draw(sf::RenderWindow &window) const;
                void setTexture(const sf::Texture &texture);

            private:
                sf::Sprite _background;
                sf::Vector2f _position;
                sf::Texture _texture;
                float _life;
        };
    }
}
