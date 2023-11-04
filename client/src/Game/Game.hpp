#pragma once

#include "../Core.hpp"
#include "./Loader/Loader.hpp"
#include "../Utils.hpp"
#include "../ECS/Registry.hpp"
#include "../Network/Network.hpp"
#include "EntitiesFactory.hpp"
#include "EntityManager/MenuManager.hpp"
#include "EntityManager/EntityManager.hpp"
#include "Fbr.hpp"

#define SERVER_WIDTH        3840
#define SERVER_HEIGHT       2160
#define SERVER_RATIO      16.0f / 9.0f
#define SOUND_VOLUME        40.0f



namespace game {
    /**
     * @brief Enum of the colors
     * 
     */
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
        /**
         * @brief Send data to server
         * 
         */
            void sendMoveToServer();
        /**
         * @brief Handle the events in the chat
         * 
         * @param deltaTime 
         */
            void handleChatInput(float deltaTime);
        /**
         * @brief Create a Main Menu Scene object
         * 
         */
            void createMainMenuScene();
        /**
         * @brief Kill the Game Entity object
         * 
         */
            void killGameEntity();
        /**
         * @brief Enum of differents game states
         * 
         */
            enum gameState {
                MENU,
                MATCHMAKING,
                GAME,
                ENDGAME
            };
        /**
         * @brief Enum of the differents missiles
         * 
         */
            enum MISSILE_TYPE {
                PLAYER_ONE = 1,
                LITTLE_MONSTER,
                ORANGE_MISSILE,
                PURPLE_MISSILE,
                GREEN_MISSILE,
                PLAYER_TWO,
                PLAYER_THREE,
                FIRE_BALL,
            };

        private:
        /**
         * @brief Framebuffer object
         * 
         */
            std::unique_ptr<Fbr> _fbr;
        /**
         * @brief Window object
         * 
         */
            sf::RenderWindow _window;
        /**
         * @brief Screen size
         * 
         */
            sf::Vector2f _screenSize;
        /**
         * @brief Real screen size (resolution)
         * 
         */
            sf::Vector2f _realScreenSize;
        /**
         * @brief Real screen size (resolution)
         * 
         */
            sf::Vector2u _realScreenSizeU;
        /**
         * @brief Strobes vector
         * 
         */
            std::vector<entity_t> _strobes;
        /**
         * @brief Song currently playing
         * 
         */
            unsigned char currentSong;
        /**
         * @brief Get the value of the top left offset
         * 
         */
            sf::Vector2u topLeftOffeset;
        /**
         * @brief Instance of the manager
         * 
         */
            Loader _manager;
        /**
         * @brief Instance of the ecs
         * 
         */
            ECS::Registry ecs;
        /**
         * @brief Instance of the factory
         * 
         */
            Factory _factory;
        /**
         * @brief Instance of the network
         * 
         */
            Network _net;
        /**
         * @brief Instance of the menu manager
         * 
         */
            MenuManager _menuManager;
        /**
         * @brief Store the last time (ping)
         * 
         */
            long _lastTime;
        /**
         * @brief Value to scale entities
         * 
         */
            float _resMult;
        /**
         * @brief Store the room id
         * 
         */
            unsigned int _roomId;
        /**
         * @brief Store the player id
         * 
         */
            unsigned int _playerId;
        /**
         * @brief Store the game state
         * 
         */
            gameState _gameState;
        /**
         * @brief Time remaining before the game start
         * 
         */
            unsigned int _startTimeLeft;
        /**
         * @brief Is the game started
         * 
         */
            unsigned char _started;
        /**
         * @brief Store the event for ControllableSystem
         * 
         */
            int eventMemory;
        /**
         * @brief Chrono of the last ping
         * 
         */
            std::chrono::system_clock::time_point _lastPing;
        /**
         * @brief Chrono of the game duration
         * 
         */
            std::chrono::system_clock::time_point _startGameTime;
        /**
         * @brief Chrono of the last time the player fire
         * 
         */
            std::chrono::system_clock::time_point _lastPlayerFireTime;
        /**
         * @brief Chrono of the last time the player fire
         * 
         */
            std::chrono::system_clock::time_point _lastPlayerBombFireTime;
        /**
         * @brief Chrono of the last time the player fire
         * 
         */
            std::chrono::system_clock::time_point _lastPlaerLaserFireTime;
        /**
         * @brief Chrono of the last time the player fire
         * 
         */
            std::chrono::system_clock::time_point _lastPlaerRayFireTime;
        /**
         * @brief Vector of the keys pressed
         * 
         */
            std::vector<u_char> _keyboardInputs;
        /**
         * @brief Message in the chat
         * 
         */
            std::string _chatInput;
        /**
         * @brief Time since last update
         * 
         */
            float _timeSinceLastUpdate;
        /**
         * @brief Vector of particles generated
         * 
         */
            std::vector<std::tuple<sf::IntRect, sf::Color, std::chrono::system_clock::time_point, size_t>> _particles;
            std::chrono::system_clock::time_point _lastParticleUpdate;
        /**
         * @brief Refresh the screen size
         * 
         */
            void refreshScreenSize();
        /**
         * @brief Init buttons
         * 
         */
            void initButtons();
        /**
         * @brief Init menus
         * 
         */
            void initMenus();
        /**
         * @brief Send the chat write by the player
         * 
         * @param msg 
         */
            void sendChat(const std::string &msg);
        /**
         * @brief Get the position of the room
         * 
         * @param roomId 
         * @return int 
         */
            int searchRoomId(int roomId);
        /**
         * @brief Handle timeout matchmaking
         * 
         * @param packet 
         */
            void handleTimeoutMatchmaking(Network::Packet &packet);
        /**
         * @brief Handle the player score
         * 
         * @param packet 
         */
            void handlePlayerScore(Network::Packet &packet);
        /**
         * @brief Hande the game end
         * 
         * @param packet 
         */
            void handleGameEnd(Network::Packet &packet);
        /**
         * @brief Handle the player position
         * 
         * @param packet 
         */
            void handlePlayerPosition(Network::Packet &packet);
        /**
         * @brief Handle the missile position
         * 
         * @param packet 
         */
            void handleMissilePosition(Network::Packet &packet);
        /**
         * @brief Handle the ennemi position
         * 
         * @param packet 
         */
            void handleEnnemiPosition(Network::Packet &packet);
        /**
         * @brief Handle ennemi death
         * 
         * @param packet 
         */
            void handleEnnemiDeath(Network::Packet &packet);
        /**
         * @brief Handle the missile death (destroyed)
         * 
         * @param packet 
         */
            void handleMissileDeath(Network::Packet &packet);
        /**
         * @brief Handle the player death
         * 
         * @param packet 
         */
            void handlePlayerDeath(Network::Packet &packet);
        /**
         * @brief Handle the player disconnected
         * 
         * @param packet 
         */
            void handlePlayerDisconnected(Network::Packet &packet);
        /**
         * @brief Handle the player joined the room
         * 
         * @param packet 
         */
            void handleRoomJoin(Network::Packet &packet);
        /**
         * @brief Handle the player joined the game
         * 
         * @param packet 
         */
            void handlePlayerJoinGame(Network::Packet &packet);
        /**
         * @brief Handle display of the player's life
         * 
         * @param packet 
         */
            void handlePlayerLife(Network::Packet &packet);
        /**
         * @brief Handle the monster's life
         * 
         * @param packet 
         */
            void handleMonsterLife(Network::Packet &packet);
        /**
         * @brief Handle strobes (lightning)
         * 
         * @param packet 
         */
            void handleStrobes(Network::Packet &packet);
        /**
         * @brief Resend the packet
         * 
         * @param packet 
         */
            void handleResend(Network::Packet &packet);
        /**
         * @brief Handle changement of level
         * 
         * @param packet 
         */
            void handleChangeLevel(Network::Packet &packet);
        /**
         * @brief Handle the latency
         * 
         * @param packet 
         */
            void handleLatency(Network::Packet &packet);
        /**
         * @brief Handle the list of rooms available
         * 
         * @param packet 
         */
            void handleListRooms(Network::Packet &packet);
        /**
         * @brief Handle bonus position
         * 
         * @param packet 
         */
            void handleBonusPosition(Network::Packet &packet);
        /**
         * @brief Handle bonus destroyed
         * 
         * @param packet 
         */
            void handleBonusDestroyed(Network::Packet &packet);
        /**
         * @brief Handle bomb position
         * 
         * @param packet 
         */
            void handleBombPosition(Network::Packet &packet);
        /**
         * @brief Handle bomb destroyed
         * 
         * @param packet 
         */
            void handleBombDestroyed(Network::Packet &packet);
        /**
         * @brief Handle the chat message in the chat
         * 
         * @param packet 
         */
            void handleChatMessage(Network::Packet &packet);
        /**
         * @brief Handle pod informations received
         * 
         * @param packet 
         */
            void handlePodInfo(Network::Packet &packet);
        /**
         * @brief Handle player fire laser
         * 
         * @param packet 
         */
            void handleLaser(Network::Packet &packet);
        /**
         * @brief Handle player fire ray
         * 
         * @param packet 
         */
            void handleRay(Network::Packet &packet);
        /**
         * @brief Generate particles for the ray
         * 
         */
            void GenParticles();
        /**
         * @brief Create a Particle object
         * 
         * @param rect 
         * @param color 
         * @param duration 
         */
            void CreateParticle(sf::IntRect rect, sf::Color color, size_t duration);
        /**
         * @brief Create a Particle object
         * 
         * @param rect 
         * @param color 
         * @param duration 
         */
            void CreateParticle(sf::FloatRect rect, sf::Color color, size_t duration);
        /**
         * @brief Handle player's bonus
         * 
         * @param packet 
         */
            void handleBonus(Network::Packet &packet);
    };
}
