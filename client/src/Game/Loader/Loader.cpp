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

    this->_textures[type] = std::make_shared<sf::Texture>();
    if (!this->_textures[type]->loadFromFile(path)) {
        throw client::MyError("Loader", "Texture cannot be loaded");
    }
}

void Loader::unloadTexture(toLoad type)
{
    if (this->_textures.find(type) == this->_textures.end())
        return; //😘

    this->_textures.erase(type);
}

const std::shared_ptr<sf::Texture> &Loader::getTexture(toLoad type) const
{
    if (this->_textures.find(type) == this->_textures.end())
        throw client::MyError("Loader", "Texture not found");
    return this->_textures.at(type);
}

void Loader::loadFont(const std::string path, toLoad type)
{
    if (this->_fonts.find(type) != this->_fonts.end())
        return; //😘

    this->_fonts[type] = std::make_shared<sf::Font>();
    if (!this->_fonts[type]->loadFromFile(path)) {
        throw client::MyError("Loader", "Font cannot be loaded");
    }
}

void Loader::unloadFont(toLoad type)
{
    if (this->_fonts.find(type) == this->_fonts.end())
        return; //😘

    this->_fonts.erase(type);
}

const std::shared_ptr<sf::Font> &Loader::getFont(toLoad type) const
{
    if (this->_fonts.find(type) == this->_fonts.end())
        throw client::MyError("Loader", "Font not found");
    return this->_fonts.at(type);
}
