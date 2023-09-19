#pragma once

#include <SFML/Graphics.hpp>
#include "../../Error/MyError.hpp"
#include "../Components/EntityAnimation.hpp"

namespace game {
    namespace entity {
        class Player {
            public:
                Player(const std::string &backgroundPath, const sf::Vector2f &firstPosition, float life);
                ~Player();

                void draw(sf::RenderWindow &window) const;
                void setTexture(const sf::Texture &texture);
                void setPosition(const sf::Vector2f &position);
                const sf::Vector2f &getPosition();
                void update(float deltaTime);
                sf::Sprite _background;

            private:
                components::EntityAnimation _animation;
                sf::Vector2f _position;
                sf::Texture _texture;
                sf::IntRect _textureRect; // TO REMOVE
                float _life;
        };
    }
}
