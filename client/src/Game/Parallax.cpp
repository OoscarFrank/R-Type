#include "Parallax.hpp"

using namespace client;

Parallax::Parallax(const std::string &backgroundPath, const sf::Vector2f &firstPosition, float speed): _position(firstPosition), _speed(speed)
{
    if (!this->_texture.loadFromFile(backgroundPath)) {
        throw MyError("Core", "Failed to load background texture");
    }

    this->_background.setTexture(this->_texture);
    this->_background.setPosition(this->_position);
}

Parallax::~Parallax()
{
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
