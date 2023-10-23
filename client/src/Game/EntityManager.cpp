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

void EntityManager::handleMusic(ECS::Registry &ecs, MUSIC_TYPE type, std::function<void(ECS::components::MusicComponent&)> callback)
{
    auto it = std::find_if(this->_musics.begin(), this->_musics.end(), [type](const auto& pair) {
        return pair.first == type;
    });
    if (it != this->_musics.end()) {
        ecs.modify_component<ECS::components::MusicComponent>(it->second, callback);
    }
}
