#pragma once

#include "MenuManager.hpp"

using entity_t = std::size_t;

namespace game {
    class EntityManager: public MenuManager {
        public:
            EntityManager();
            ~EntityManager();

        enum MUSIC_TYPE {
            SOUND_OF_SPACE = 1,
            TURN_ON_THE_LIGHTS,
            PUSH_UP,
            VOIS_SUR_TON_CHEMIN

        };

        protected:
            entity_t getPlayerEntityFromId(unsigned int id);
            entity_t getMissileEntityFromId(unsigned int id);
            entity_t getEnnemiEntityFromId(unsigned int id);
            void handleMusic(ECS::Registry &ecs, MUSIC_TYPE type, std::function<void(ECS::components::MusicComponent&)> callback);

            std::unordered_map<MUSIC_TYPE, entity_t> _musics;
            std::vector<std::pair<size_t, entity_t>> _players;
            std::vector<std::pair<size_t, entity_t>> _missiles;
            std::vector<std::pair<size_t, entity_t>> _ennemies;
            std::vector<ECS::systems::MovableSystem::EntityPos> _entityPositions;
            std::vector<ECS::systems::ControllableSystem::EntityEvent> _entityEvents;
            std::vector<entity_t> _parallax;
            std::vector<entity_t> _looser;
            entity_t _playerEntity;
            entity_t _playerLife;
            entity_t _scoreCoche;
    };
}