#pragma once

#include "../../ECS/Systems/Systems.hpp"

using entity_t = std::size_t;

namespace game {
    class EntityManager {
        public:
            EntityManager();
            ~EntityManager();

        enum MUSIC_TYPE {
            SOUND_OF_SPACE = 1,
            TURN_ON_THE_LIGHTS,
            PUSH_UP,
            VOIS_SUR_TON_CHEMIN,
            HEUTE_NACHT,
            CLEON,
            AMNESIA
        };

        enum SOUND_TYPE {
            TEST
        };

        enum LOADINGBAR_TYPE {
            PLAYER_LIFE
        };

        enum SCREEN_TYPE {
            MAIN_MENU
        };

        protected:
            entity_t getPlayerEntityFromId(unsigned int id);
            entity_t getMissileEntityFromId(unsigned int id);
            entity_t getEnnemiEntityFromId(unsigned int id);
            entity_t getBonusEntityFromId(unsigned int id);
            entity_t getBombEntityFromId(unsigned int id);
            void handleMusic(ECS::Registry &ecs, MUSIC_TYPE type, std::function<void(ECS::components::MusicComponent&)> callback);

            std::unordered_map<LOADINGBAR_TYPE, entity_t> _loadingBar;
            std::unordered_map<MUSIC_TYPE, entity_t> _musics;
            std::unordered_map<SOUND_TYPE, entity_t> _sounds;
            std::vector<std::pair<size_t, entity_t>> _players;
            std::vector<std::pair<size_t, entity_t>> _missiles;
            std::vector<std::pair<size_t, entity_t>> _ennemies;
            std::vector<std::pair<size_t, entity_t>> _bonuses;
            std::vector<std::pair<size_t, entity_t>> _bombs;
            
            std::vector<ECS::systems::MovableSystem::EntityPos> _entityPositions;
            std::vector<ECS::systems::ControllableSystem::EntityEvent> _entityEvents;
            std::vector<entity_t> _parallax;

            std::vector<std::tuple<int, entity_t, entity_t>> _roomsData; // roomId, text1, text2
            std::map<SCREEN_TYPE, entity_t> _screens;
            entity_t _looser;
            entity_t _playerEntity;
            entity_t _scoreCoche;
    };
}
