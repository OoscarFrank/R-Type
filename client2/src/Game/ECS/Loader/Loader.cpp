#include "Loader.hpp"

using namespace game;

Loader::Loader()
{
    loadTexture("background.png", Loader::toLoad::ParallaxFirstbkg);
    loadTexture("background2.png", Loader::toLoad::ParallaxSecondbkg);
    loadTexture("player/move.png", Loader::toLoad::Player);
    loadTexture("rocket.png", Loader::toLoad::Rocket);
    loadTexture("monsters/monster1.png", Loader::toLoad::Monster1);
}

Loader::~Loader()
{
}

void Loader::loadTexture(const std::string path, toLoad type)
{
    if (this->_textures.find(type) != this->_textures.end())
        return; //😘

    this->_textures[type] = std::make_unique<sf::Texture>();
    this->_textures[type]->loadFromFile(path);
}

const sf::Texture &Loader::getTexture(toLoad type) const
{
    if (this->_textures.find(type) == this->_textures.end())
        throw client::MyError("Loader", "Texture not found");
    return *this->_textures.at(type);
}
