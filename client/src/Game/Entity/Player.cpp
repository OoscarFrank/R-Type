#include "Player.hpp"

using namespace game;
using namespace entity;

Player::Player(const sf::Vector2f &firstPosition, float life):
    _position(firstPosition),
    _life(life),
    _animation(5, 8.0f, {113, 64})
{
    this->_background.setPosition(this->_position);
    this->_background.setTextureRect(sf::IntRect(0, 0, 113, 64));
}

Player::~Player()
{
}

void Player::setTexture(const sf::Texture &texture)
{
    this->_background.setTexture(texture);
}

void Player::setPosition(const sf::Vector2f &position)
{
    this->_position = position;
    this->_background.setPosition(this->_position);
}

const sf::Vector2f &Player::getPosition()
{
    return this->_position;
}

void Player::update(float deltaTime)
{
    _animation.update(deltaTime, this->_background);
}


void Player::draw(sf::RenderWindow &window) const
{
    window.draw(this->_background);
}
