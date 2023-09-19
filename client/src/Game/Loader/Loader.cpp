#include "Loader.hpp"

using namespace game;
using namespace loader;

Loader::Loader()
{
    loadTexture("./client/assets/parallax/background.png", Loader::toLoad::ParallaxFirstbkg);
    loadTexture("./client/assets/parallax/background2.png", Loader::toLoad::ParallaxSecondbkg);
    loadTexture("./client/assets/entity/player/Move.png", Loader::toLoad::Player);
    loadTexture("./client/assets/entity/rocket.png", Loader::toLoad::Rocket);
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
