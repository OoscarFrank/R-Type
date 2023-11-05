#include "EntityManager.hpp"

using namespace game;

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

entity_t EntityManager::getPlayerEntityFromId(unsigned int id)
{
    for (auto &player : this->_players)
    {
        if (player.first == id)
            return player.second;
    }
    return 0;
}

entity_t EntityManager::getMissileEntityFromId(unsigned int id)
{
    for (auto &missile : this->_missiles)
    {
        if (missile.first == id)
            return missile.second;
    }
    return 0;
}

entity_t EntityManager::getEnnemiEntityFromId(unsigned int id)
{
    for (auto &ennemi : this->_ennemies)
    {
        if (ennemi.first == id)
            return ennemi.second;
    }
    return 0;
}

entity_t EntityManager::getTextByType(TEXT_TYPE type)
{
    for (auto &text : this->_textsEntity) {
        if (text.first == type)
            return text.second;
    }
    return 0;
}

void EntityManager::stopAllMusic(ECS::Registry &ecs)
{
    for (auto &music : this->_musics) {
        ecs.modify_component<ECS::components::MusicComponent>(music.second, [](ECS::components::MusicComponent &music) {
            music.stopmusic();
        });
    }
}

void EntityManager::handleMusic(ECS::Registry &ecs, MUSIC_TYPE type, std::function<void(ECS::components::MusicComponent&)> callback)
{
    auto it = std::find_if(this->_musics.begin(), this->_musics.end(), [type](const auto& pair) {
        return pair.first == type;
    });
    if (it != this->_musics.end()) {
        ecs.modify_component<ECS::components::MusicComponent>(it->second, callback);
    }
}

entity_t EntityManager::getBonusEntityFromId(unsigned int id)
{
    for (auto &bonus : this->_bonuses)
    {
        if (bonus.first == id)
            return bonus.second;
    }
    return 0;
}

entity_t EntityManager::getBombEntityFromId(unsigned int id)
{
    for (auto &bomb : this->_bombs)
    {
        if (bomb.first == id)
            return bomb.second;
    }
    return 0;
}


std::pair<entity_t, unsigned char> EntityManager::getPodEntityFromId(unsigned int id)
{
    for (auto &pod : this->_pods)
    {
        if (std::get<0>(pod) == id) {
            std::pair<entity_t, unsigned char> out(std::get<1>(pod), std::get<2>(pod));
            return out;
        }
    }
    return std::make_pair(0, 0);
}


sf::Vector2f EntityManager::getRayEntityFromId(unsigned int id)
{
    for (auto &ray : this->_rays)
    {
        if (ray.first == id) {
            return ray.second;
        }
    }
    return sf::Vector2f(-100, -100);
}