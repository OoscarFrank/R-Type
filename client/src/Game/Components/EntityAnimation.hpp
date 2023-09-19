#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace game {
    namespace components {
        class EntityAnimation {
            public:
                EntityAnimation(const unsigned &iteration, const float &speed, const sf::Vector2i &size);
                ~EntityAnimation();

                void update(float deltaTime, sf::Sprite &texture);
                unsigned getCurrentIteration() const;

            private:
                unsigned _nbrIteration;
                float _speed;
                float _timeElapsed;
                unsigned _iteration;
                sf::Vector2i _size;
        };
    }
}
