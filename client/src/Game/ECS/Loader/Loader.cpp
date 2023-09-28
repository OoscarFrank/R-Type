#include "Loader.hpp"

using namespace game;

Loader::Loader()
{
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

void Loader::unloadTexture(toLoad type)
{
    if (this->_textures.find(type) == this->_textures.end())
        return; //😘

    this->_textures.erase(type);
}

const sf::Texture &Loader::getTexture(toLoad type) const
{
    if (this->_textures.find(type) == this->_textures.end())
        throw client::MyError("Loader", "Texture not found");
    return *this->_textures.at(type);
}
