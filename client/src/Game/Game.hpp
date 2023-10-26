#pragma once

#include "../Core.hpp"
#include "./Loader/Loader.hpp"
#include "../Utils.hpp"
#include "../ECS/Registry.hpp"
#include "../Network/Network.hpp"
#include "EntitiesFactory.hpp"
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


            std::unordered_map<entity_t, std::string> _texts;
            entity_t _timerText = 0;

            Loader _manager;
            ECS::Registry ecs;
            Factory _factory;
            Network _net;

            long _lastTime;
            float _resMult;

            unsigned int _roomId;
            unsigned int _playerId;
            bool _gameOver;

            unsigned int _startTimeLeft;
            unsigned char _started;

            int eventMemory;
            std::chrono::system_clock::time_point _lastPing;

            void refreshScreenSize();

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
            void handleStrobes(Network::Packet &packet);
            void handleResend(Network::Packet &packet);
            void handleChangeLevel(Network::Packet &packet);
    };
}
