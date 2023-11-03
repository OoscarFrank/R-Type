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
            AMNESIA,
            SEVENNATION,
            BLAHBLAH,
            LOBBY,
            MATCHMAKING
        };

        enum LOADINGBAR_TYPE {
            PLAYER_LIFE
        };

        enum SCREEN_TYPE {
            MAIN_MENU,
            CHAT_BOX
        };

        enum TEXT_TYPE {
            TIMER,
            SCORE,
            GAME_TIME,
            PING,
            TCHAT
        };

        protected:
            entity_t getPlayerEntityFromId(unsigned int id);
            entity_t getMissileEntityFromId(unsigned int id);
            entity_t getEnnemiEntityFromId(unsigned int id);
            entity_t getBonusEntityFromId(unsigned int id);
            entity_t getBombEntityFromId(unsigned int id);
            std::pair<entity_t, unsigned char> getPodEntityFromId(unsigned int id);
            entity_t getTextByType(TEXT_TYPE type);

            void stopAllMusic(ECS::Registry &ecs);
            void handleMusic(ECS::Registry &ecs, MUSIC_TYPE type, std::function<void(ECS::components::MusicComponent&)> callback);

            std::unordered_map<LOADINGBAR_TYPE, entity_t> _loadingBar;
            std::unordered_map<MUSIC_TYPE, entity_t> _musics;
            std::vector<entity_t> _sounds;
            std::vector<std::pair<size_t, entity_t>> _players;
            std::vector<std::pair<size_t, entity_t>> _missiles;
            std::vector<std::pair<size_t, entity_t>> _ennemies;
            std::vector<std::pair<size_t, entity_t>> _bonuses;
            std::vector<std::pair<size_t, entity_t>> _bombs;
            std::vector<std::tuple<size_t, entity_t, unsigned char>> _pods;

            std::vector<ECS::systems::MovableSystem::EntityPos> _entityPositions;
            std::vector<ECS::systems::ControllableSystem::EntityEvent> _entityEvents;
            std::vector<entity_t> _parallax;
            std::vector<std::tuple<int, entity_t, entity_t>> _roomsData; // roomId, text1, text2
            std::map<SCREEN_TYPE, entity_t> _screens;
            std::unordered_map<entity_t, std::string> _textsUpdate;
            std::unordered_map<TEXT_TYPE, entity_t> _textsEntity;
            std::vector<entity_t> _textChat;

            entity_t _looser;
            entity_t _playerEntity;
            entity_t _scoreCoche;

            entity_t _blackBandTopLeft;
            entity_t _blackBandBottomRight;
    };
}
