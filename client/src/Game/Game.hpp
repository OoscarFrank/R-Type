#pragma once

#include "../Core.hpp"
#include "./Loader/Loader.hpp"
#include "../Utils.hpp"
#include "../ECS/Registry.hpp"
#include "../Network/Network.hpp"
#include "EntitiesFactory.hpp"
#include "EntityManager/MenuManager.hpp"
#include "EntityManager/EntityManager.hpp"

#define SERVER_WIDTH        3840
#define SERVER_HEIGHT       2160
#define SERVER_RATIO      16.0f / 9.0f



namespace game {
    enum Colors {
        RED = 1,
        GREEN,
        BLUE,
        YELLOW,
        PURPLE,
        CYAN,
        WHITE
    };
    class Game: public EntityManager {
        public:
        /**
         * @brief Construct a new Game object
         *
         */
            Game(std::string ip, int port);
        /**
         * @brief Destroy the Game object
         *
         */
            ~Game();
        /**
         * @brief function who's looping on the window and call everythings
         *
         * @return int
         */
            int MainLoop();
        /**
         * @brief update data for entities
         *
         */
            void update();
            void sendMoveToServer();

            void createMainMenuScene();

            void killGameEntity();

            enum gameState {
                MENU,
                MATCHMAKING,
                GAME,
                ENDGAME
            };

        private:
            sf::RenderWindow _window;
            sf::Vector2f _screenSize;
            sf::Vector2f _realScreenSize;
            sf::Vector2u _realScreenSizeU;

            std::vector<entity_t> _strobes;
            unsigned char currentSong;

            sf::Vector2u topLeftOffeset;

            entity_t _blackBandTopLeft;
            entity_t _blackBandBottomRight;

            Loader _manager;
            ECS::Registry ecs;
            Factory _factory;
            Network _net;
            MenuManager _menuManager;

            long _lastTime;
            float _resMult;

            unsigned int _roomId;
            unsigned int _playerId;
            gameState _gameState;

            unsigned int _startTimeLeft;
            unsigned char _started;

            int eventMemory;
            std::chrono::system_clock::time_point _lastPing;
            std::chrono::system_clock::time_point _startGameTime;
            std::chrono::system_clock::time_point _lastPlayerFireTime;
            std::chrono::system_clock::time_point _lastPlayerBombFireTime;

            void refreshScreenSize();

            void initButtons();
            void initMenus();
            void sendChat(const std::string &msg);

            int searchRoomId(int roomId);

            void handleTimeoutMatchmaking(Network::Packet &packet);
            void handlePlayerScore(Network::Packet &packet);
            void handleGameEnd(Network::Packet &packet);
            void handlePlayerPosition(Network::Packet &packet);
            void handleMissilePosition(Network::Packet &packet);
            void handleEnnemiPosition(Network::Packet &packet);
            void handleEnnemiDeath(Network::Packet &packet);
            void handleMissileDeath(Network::Packet &packet);
            void handlePlayerDeath(Network::Packet &packet);
            void handlePlayerDisconnected(Network::Packet &packet);
            void handleRoomJoin(Network::Packet &packet);
            void handlePlayerJoinGame(Network::Packet &packet);
            void handlePlayerLife(Network::Packet &packet);
            void handleMonsterLife(Network::Packet &packet);
            void handleStrobes(Network::Packet &packet);
            void handleResend(Network::Packet &packet);
            void handleChangeLevel(Network::Packet &packet);
            void handleLatency(Network::Packet &packet);
            void handleListRooms(Network::Packet &packet);
            void handleBonusPosition(Network::Packet &packet);
            void handleBonusDestroyed(Network::Packet &packet);
            void handleBombPosition(Network::Packet &packet);
            void handleBombDestroyed(Network::Packet &packet);
            void handleChatMessage(Network::Packet &packet);
            void handlePodInfo(Network::Packet &packet);
            
    };
}
