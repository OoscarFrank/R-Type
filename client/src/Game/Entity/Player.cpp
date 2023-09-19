#include "Player.hpp"

using namespace game;
using namespace entity;

Player::Player(const std::string &backgroundPath, const sf::Vector2f &firstPosition, float life): _position(firstPosition), _life(life)
{
    if (!this->_texture.loadFromFile(backgroundPath)) {
        throw client::MyError("Core", "Failed to load background texture");
    }

    // this->_background.setTexture(this->_texture);
    this->_background.setPosition(this->_position);
}

void Player::setTexture(const sf::Texture &texture)
{
    this->_background.setTexture(texture);
}

void Player::setPosition(const sf::Vector2f &position)
{
    std::cout << "Player::setPosition" << std::endl;
    this->_position = position;
}

const sf::Vector2f &Player::getPosition()
{
    return this->_position;
}

Player::~Player()
{
}

void Player::draw(sf::RenderWindow &window) const
{
    window.draw(this->_background);
}
