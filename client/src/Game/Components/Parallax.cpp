#include "Parallax.hpp"

using namespace game;
using namespace components;

Parallax::Parallax(const sf::Vector2f &firstPosition, float speed): _position(firstPosition), _speed(speed)
{
    this->_background.setPosition(this->_position);
}

Parallax::~Parallax()
{
}

void Parallax::setTexture(const sf::Texture &texture)
{
    this->_background.setTexture(texture);
}

void Parallax::update(float deltaTime)
{
    this->_position.x -= this->_speed * deltaTime;
    if (this->_position.x < -this->_background.getGlobalBounds().width / 2) {
        this->_position.x = 0;
    }
    this->_background.setPosition(this->_position);
}

void Parallax::draw(sf::RenderWindow &window) const
{
    window.draw(this->_background);
}
