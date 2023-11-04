#pragma once

#include "../../ECS/Systems/Systems.hpp"

using entity_t = std::size_t;

namespace game {
    class EntityManager {
        public:
        /**
         * @brief Construct a new Entity Manager object
         * 
         */
            EntityManager();
        /**
         * @brief Destroy the Entity Manager object
         * 
         */
            ~EntityManager();
        /**
         * @brief Enum of the differents musics
         * 
         */
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
        /**
         * @brief Enum of the loading bar available
         * 
         */
        enum LOADINGBAR_TYPE {
            PLAYER_LIFE
        };
        /**
         * @brief Enum of the differents type of screen
         * 
         */
        enum SCREEN_TYPE {
            MAIN_MENU,
            CHAT_BOX
        };
        /**
         * @brief Enum of the differents type of text
         * 
         */
        enum TEXT_TYPE {
            TIMER,
            SCORE,
            GAME_TIME,
            PING,
            TCHAT
        };

        protected:
        /**
         * @brief Get the Player Entity From Id object
         * 
         * @param id 
         * @return entity_t 
         */
            entity_t getPlayerEntityFromId(unsigned int id);
        /**
         * @brief Get the Missile Entity From Id object
         * 
         * @param id 
         * @return entity_t 
         */
            entity_t getMissileEntityFromId(unsigned int id);
        /**
         * @brief Get the Ennemi Entity From Id object
         * 
         * @param id 
         * @return entity_t 
         */
            entity_t getEnnemiEntityFromId(unsigned int id);
        /**
         * @brief Get the Bonus Entity From Id object
         * 
         * @param id 
         * @return entity_t 
         */
            entity_t getBonusEntityFromId(unsigned int id);
        /**
         * @brief Get the Bomb Entity From Id object
         * 
         * @param id 
         * @return entity_t 
         */
            entity_t getBombEntityFromId(unsigned int id);
        /**
         * @brief Get the Pod Entity From Id object
         * 
         * @param id 
         * @return std::pair<entity_t, unsigned char> 
         */
            std::pair<entity_t, unsigned char> getPodEntityFromId(unsigned int id);
        /**
         * @brief Get the Ray Entity From Id object
         * 
         * @param id 
         * @return sf::Vector2f 
         */
            sf::Vector2f getRayEntityFromId(unsigned int id);
        /**
         * @brief Get the Text By Type object
         * 
         * @param type 
         * @return entity_t 
         */
            entity_t getTextByType(TEXT_TYPE type);
        /**
         * @brief Stop all the music
         * 
         * @param ecs 
         */
            void stopAllMusic(ECS::Registry &ecs);
        /**
         * @brief Handle the music behavior
         * 
         * @param ecs 
         * @param type 
         * @param callback 
         */
            void handleMusic(ECS::Registry &ecs, MUSIC_TYPE type, std::function<void(ECS::components::MusicComponent&)> callback);
        /**
         * @brief Store the loading bar
         * 
         */
            std::unordered_map<LOADINGBAR_TYPE, entity_t> _loadingBar;
        /**
         * @brief Store the musics
         * 
         */
            std::unordered_map<MUSIC_TYPE, entity_t> _musics;
        /**
         * @brief Store the sounds
         * 
         */
            std::vector<entity_t> _sounds;
        /**
         * @brief Store the players
         * 
         */
            std::vector<std::pair<size_t, entity_t>> _players;
        /**
         * @brief Store the missiles
         * 
         */
            std::vector<std::pair<size_t, entity_t>> _missiles;
        /**
         * @brief Store the ennemies
         * 
         */
            std::vector<std::pair<size_t, entity_t>> _ennemies;
        /**
         * @brief Store the bonuses
         * 
         */
            std::vector<std::pair<size_t, entity_t>> _bonuses;
        /**
         * @brief Store the bombs
         * 
         */
            std::vector<std::pair<size_t, entity_t>> _bombs;
        /**
         * @brief Store the pods
         * 
         */
            std::vector<std::tuple<size_t, entity_t, unsigned char>> _pods;
        /**
         * @brief Store the rays
         * 
         */
            std::vector<std::pair<size_t, sf::Vector2f>> _rays;
        /**
         * @brief Store the entities positions
         * 
         */
            std::vector<ECS::systems::MovableSystem::EntityPos> _entityPositions;
        /**
         * @brief Store the entities events
         * 
         */
            std::vector<ECS::systems::ControllableSystem::EntityEvent> _entityEvents;
        /**
         * @brief Store the entities parallax
         * 
         */
            std::vector<entity_t> _parallax;
        /**
         * @brief Store the rooms data
         * 
         */
            std::vector<std::tuple<int, entity_t, entity_t>> _roomsData; // roomId, text1, text2
        /**
         * @brief Store the different screens
         * 
         */
            std::map<SCREEN_TYPE, entity_t> _screens;
        /**
         * @brief Store the different texts
         * 
         */
            std::unordered_map<entity_t, std::string> _textsUpdate;
        /**
         * @brief Store the different texts
         * 
         */
            std::unordered_map<TEXT_TYPE, entity_t> _textsEntity;
        /**
         * @brief Store the different texts
         * 
         */
            std::vector<entity_t> _textChat;
        /**
         * @brief Looser entity
         * 
         */
            entity_t _looser;
        /**
         * @brief Player entity
         * 
         */
            entity_t _playerEntity;
        /**
         * @brief Score coche entity
         * 
         */
            entity_t _scoreCoche;
        /**
         * @brief Black band top left entity
         * 
         */
            entity_t _blackBandTopLeft;
        /**
         * @brief Black band top right entity
         * 
         */
            entity_t _blackBandBottomRight;
    };
}
