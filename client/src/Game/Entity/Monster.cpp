#include "Monster.hpp"

using namespace game;
using namespace entity;

Monster::Monster(const sf::Vector2f &firstPosition, float life):
    _position(firstPosition),
    _life(life),
    _animation(6, 8.0f, {192, 192})
{
    this->_background.setPosition(this->_position);
    this->_background.setTextureRect(sf::IntRect(0, 0, 192, 192));
}

Monster::~Monster()
{
}

void Monster::setTexture(const sf::Texture &texture)
{
    this->_background.setTexture(texture);
}

void Monster::setPosition(const sf::Vector2f &position)
{
    this->_position = position;
    this->_background.setPosition(this->_position);
}

const sf::Vector2f &Monster::getPosition()
{
    return this->_position;
}

void Monster::update(float deltaTime)
{
    _animation.update(deltaTime, this->_background);
}


void Monster::draw(sf::RenderWindow &window) const
{
    window.draw(this->_background);
}
