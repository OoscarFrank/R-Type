#include "Rocket.hpp"

using namespace game::entity;

Rocket::Rocket(const sf::Vector2f &firstPosition, const loader::Loader &loader): _position(firstPosition), _speed(300.0) {
    _sprite.setPosition(this->_position);
    _sprite.setTexture(loader.getTexture(loader::Loader::toLoad::Rocket));
}

Rocket::~Rocket() {}

void Rocket::draw(sf::RenderWindow &window) const {
    window.draw(_sprite);
}

void Rocket::setTexture(const sf::Texture &texture) {
    _sprite.setTexture(texture);
}

void Rocket::setPosition(const sf::Vector2f &position) {
    this->_position = position;
    _sprite.setPosition(this->_position);
}

const sf::Vector2f &Rocket::getPosition() {
    return _sprite.getPosition();
}

void Rocket::update(float deltaTime) {
    this->_position.x += this->_speed * deltaTime;
    this->_sprite.setPosition(this->_position);
}
