#include "EntityAnimation.hpp"

using namespace game::components;

EntityAnimation::EntityAnimation(const unsigned &iteration, const float &speed, const sf::Vector2i &size)
    : _nbrIteration(iteration), _speed(speed), _timeElapsed(0.0f), _iteration(0), _size(size)
{
}

EntityAnimation::~EntityAnimation()
{
}

void EntityAnimation::update(float deltaTime, sf::Sprite &sprite)
{
    _timeElapsed += deltaTime;

    float iterationDuration = 1.0f / _speed;
    _iteration = static_cast<unsigned>(_timeElapsed / iterationDuration);

    if (_iteration >= _nbrIteration) {
        _iteration = 0;
        _timeElapsed = 0.0f;
    }

    int spriteIndex = this->getCurrentIteration();

    int x = spriteIndex * _size.x;

    sprite.setTextureRect(sf::IntRect(x, 0, _size.x, _size.y));
}

unsigned EntityAnimation::getCurrentIteration() const
{
    return _iteration;
}
