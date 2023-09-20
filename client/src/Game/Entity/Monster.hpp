#pragma once

#include <SFML/Graphics.hpp>
#include "../../Error/MyError.hpp"
#include "../Components/EntityAnimation.hpp"

namespace game {
    namespace entity {
        class Monster {
            public:
                Monster(const sf::Vector2f &firstPosition, float life);
                ~Monster();

                void draw(sf::RenderWindow &window) const;
                void setTexture(const sf::Texture &texture);
                void setPosition(const sf::Vector2f &position);
                const sf::Vector2f &getPosition();
                void update(float deltaTime);
                sf::Sprite _background;

            private:
                sf::Vector2f _position;
                sf::Texture _texture;
                float _life;
                components::EntityAnimation _animation;
        };
    }
}
