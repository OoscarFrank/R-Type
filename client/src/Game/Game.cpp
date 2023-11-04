#include "./Game.hpp"
#include "../ECS/Components/Components.hpp"
#include "../ECS/Systems/Systems.hpp"
#include <iomanip>

using namespace game;
using entity_t = std::size_t;
using namespace TypesLitterals;

Game::Game(std::string ip, int port) :
    _manager(Loader()),
    _factory(Factory(ecs)),
    _net(Network(ip, port)),
    _menuManager(ecs),
    _roomId(0),
    _playerId(0),
    _startTimeLeft(0),
    _started(0),
    _timeSinceLastUpdate(0.0f),
    _lastPing(std::chrono::system_clock::now()),
    _lastPlayerFireTime(std::chrono::system_clock::now())
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    try {
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/BlackPixel.png"), Loader::toLoad::BlackPixel);
        this->_manager.loadTexture(client::getAssetPath("parallax/background.png"), Loader::toLoad::ParallaxFirstbkg);
        this->_manager.loadTexture(client::getAssetPath("parallax/background2.png"), Loader::toLoad::ParallaxSecondbkg);
        this->_manager.loadTexture(client::getAssetPath("entity/buttons/CreateRoomButton.png"), Loader::toLoad::CreateRoomButton);
        this->_manager.loadTexture(client::getAssetPath("entity/buttons/JoinRoomButton.png"), Loader::toLoad::JoinRoomButton);
        this->_manager.loadTexture(client::getAssetPath("entity/buttons/QuitButton.png"), Loader::toLoad::QuitButton);
        this->_manager.loadTexture(client::getAssetPath("entity/buttons/LeaveGame.png"), Loader::toLoad::LeaveGame);
        this->_manager.loadTexture(client::getAssetPath("entity/buttons/matchlist.png"), Loader::toLoad::MatchListButton);

        this->_manager.loadTexture(client::getAssetPath("screens/LooserScreen.png"), Loader::toLoad::LooserScreen);
        this->_manager.loadTexture(client::getAssetPath("screens/menuScreen.png"), Loader::toLoad::MenuScreen);
        this->_manager.loadTexture(client::getAssetPath("screens/chatbox.png"), Loader::toLoad::ChatBox);

        this->_manager.loadTexture(client::getAssetPath("entity/missile/missile.png"), Loader::toLoad::Missile);
        this->_manager.loadTexture(client::getAssetPath("entity/missile/missileRed.png"), Loader::toLoad::MissileRed);
        this->_manager.loadTexture(client::getAssetPath("entity/missile/orange_missile.png"), Loader::toLoad::OrangeMissile);
        this->_manager.loadTexture(client::getAssetPath("entity/missile/purple_missile.png"), Loader::toLoad::PurpleMissile);
        this->_manager.loadTexture(client::getAssetPath("entity/missile/green_missile.png"), Loader::toLoad::GreenMissile);
        this->_manager.loadTexture(client::getAssetPath("entity/missile/fireBall.png"), Loader::toLoad::FireBall);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/monster1.png"), Loader::toLoad::Monster1);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/mob1.png"), Loader::toLoad::Monster2);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/mob2.png"), Loader::toLoad::Monster3);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/mob3.png"), Loader::toLoad::Monster4);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/boss1.png"), Loader::toLoad::Boss1);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/boss2.png"), Loader::toLoad::Boss2);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/boss1.png"), Loader::toLoad::Boss3);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/boss1.png"), Loader::toLoad::Boss4);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/boss1.png"), Loader::toLoad::Boss5);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/boss1.png"), Loader::toLoad::Boss6);
        this->_manager.loadTexture(client::getAssetPath("entity/player/player_move1.png"), Loader::toLoad::Player_move1);
        this->_manager.loadTexture(client::getAssetPath("entity/player/player_move2.png"), Loader::toLoad::Player_move2);
        this->_manager.loadTexture(client::getAssetPath("entity/player/player_move3.png"), Loader::toLoad::Player_move3);
        this->_manager.loadTexture(client::getAssetPath("entity/player/player_move4.png"), Loader::toLoad::Player_move4);
        this->_manager.loadTexture(client::getAssetPath("entity/buttons/ScoreCoche.png"), Loader::toLoad::ScoreCoche);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/RedPixel.png"), Loader::toLoad::RedPixel);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/GreenPixel.png"), Loader::toLoad::GreenPixel);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/BluePixel.png"), Loader::toLoad::BluePixel);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/CyanPixel.png"), Loader::toLoad::CyanPixel);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/PurplePixel.png"), Loader::toLoad::PurplePixel);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/YellowPixel.png"), Loader::toLoad::YellowPixel);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/WhitePixel.png"), Loader::toLoad::WhitePixel);
        this->_manager.loadTexture(client::getAssetPath("entity/bonus/bonus.png"), Loader::toLoad::Bonus);
        this->_manager.loadTexture(client::getAssetPath("entity/missile/bomb.png"), Loader::toLoad::Bomb);
        this->_manager.loadTexture(client::getAssetPath("entity/bonus/forcePodOne.png"), Loader::toLoad::Pod1);
        this->_manager.loadTexture(client::getAssetPath("entity/bonus/forcePodTwo.png"), Loader::toLoad::Pod2);
        this->_manager.loadTexture(client::getAssetPath("entity/bonus/forcePodThree.png"), Loader::toLoad::Pod3);
        this->_manager.loadTexture(client::getAssetPath("entity/missile/explosion1.png"), Loader::toLoad::Explosion1);
        this->_manager.loadTexture(client::getAssetPath("entity/missile/laser.png"), Loader::toLoad::Laser);

        this->_manager.loadTexture(client::getAssetPath("entity/player/PlayerLifeOutline.png"), Loader::toLoad::playerLifeOutline);
        this->_manager.loadTexture(client::getAssetPath("entity/player/playerLifeContent.png"), Loader::toLoad::playerLifeContent);

        this->_manager.loadFont(client::getAssetPath("fonts/arial.ttf"), Loader::toLoad::Arial);
        this->_manager.loadFont(client::getAssetPath("fonts/PressStart2P-Regular.ttf"), Loader::toLoad::PressStart2P);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(84);
    }

    if (mode.isValid()) {
        this->_window.create(mode, "R-TYPE", sf::Style::Fullscreen);
    } else {
        this->_window.create(sf::VideoMode(mode.width, mode.height), "R-TYPE");
    }
    this->refreshScreenSize();
    this->_window.setFramerateLimit(120);
    this->_resMult = static_cast<float>(this->_screenSize.x)/ SCREEN_WIDTH;

    this->_lastTime = NOW;
    this->eventMemory = 0;

    this->_roomsData.resize(6);
    for (auto &node : this->_roomsData) {
        node = std::make_tuple(-1, 0, 0);
    }

    float soundLevel_volume = SOUND_VOLUME;
    this->_musics.emplace(EntityManager::MUSIC_TYPE::SOUND_OF_SPACE, this->_factory.createMusic(client::getAssetPath("songs/levels/SOUND_OF_SPACE.ogg"), soundLevel_volume, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::TURN_ON_THE_LIGHTS, this->_factory.createMusic(client::getAssetPath("songs/levels/TURN_ON_THE_LIGHTS.ogg"), soundLevel_volume, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::PUSH_UP, this->_factory.createMusic(client::getAssetPath("songs/levels/PUSH_UP.ogg"), soundLevel_volume, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::VOIS_SUR_TON_CHEMIN, this->_factory.createMusic(client::getAssetPath("songs/levels/VOIS_SUR_TON_CHEMIN.ogg"), soundLevel_volume, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::HEUTE_NACHT, this->_factory.createMusic(client::getAssetPath("songs/levels/HEUTE_NACHT.ogg"), soundLevel_volume, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::CLEON, this->_factory.createMusic(client::getAssetPath("songs/levels/CLEON.ogg"), soundLevel_volume, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::AMNESIA, this->_factory.createMusic(client::getAssetPath("songs/levels/AMNESIA.ogg"), soundLevel_volume, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::SEVENNATION, this->_factory.createMusic(client::getAssetPath("songs/levels/SEVENNATION.ogg"), soundLevel_volume, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::BLAHBLAH, this->_factory.createMusic(client::getAssetPath("songs/levels/BLAHBLAH.ogg"), soundLevel_volume, true));

    this->_musics.emplace(EntityManager::MUSIC_TYPE::LOBBY, this->_factory.createMusic(client::getAssetPath("songs/ambient/lobby.ogg"), soundLevel_volume, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::MATCHMAKING, this->_factory.createMusic(client::getAssetPath("songs/ambient/matchmaking.ogg"), soundLevel_volume, true));

    this->_parallax.push_back(this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxFirstbkg), (-0.070f * _resMult), sf::Vector2f(_resMult, _resMult), _resMult));
    this->_parallax.push_back(this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxSecondbkg), (-0.1f * _resMult), sf::Vector2f(_resMult, _resMult), _resMult));

    this->initButtons();
    this->initMenus();

    this->_strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::RedPixel), this->_screenSize.x, this->_screenSize.y));
    this->_strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::GreenPixel), this->_screenSize.x, this->_screenSize.y));
    this->_strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::BluePixel), this->_screenSize.x, this->_screenSize.y));
    this->_strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::YellowPixel), this->_screenSize.x, this->_screenSize.y));
    this->_strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::PurplePixel), this->_screenSize.x, this->_screenSize.y));
    this->_strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::CyanPixel), this->_screenSize.x, this->_screenSize.y));
    this->_strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::WhitePixel), this->_screenSize.x, this->_screenSize.y));

    if (this->getTextByType(game::EntityManager::TEXT_TYPE::PING)) {
        entity_t newEntity = this->_factory.createText("Ping: 0", this->_manager.getFont(Loader::Loader::PressStart2P), this->_screenSize.x - 150, this->_screenSize.y - 30, 14);
        this->_textsEntity.insert({game::EntityManager::TEXT_TYPE::PING, newEntity});
    }
    this->createMainMenuScene();
    this->_fbr = std::make_unique<Fbr>(this->_screenSize.x, this->_screenSize.y);
    entity_t fbrEntity = this->ecs.spawn_entity(90);
    this->ecs.emplace_component<ECS::components::SpriteComponent>(fbrEntity, ECS::components::SpriteComponent{this->_fbr->getTexture()});
    this->ecs.emplace_component<ECS::components::PositionComponent>(fbrEntity, ECS::components::PositionComponent{0, 0});
}

Game::~Game()
{
}

void Game::createMainMenuScene()
{
    this->_gameState = gameState::MENU;

    this->_menuManager.disableAllmenu();

    for (auto &e : this->_parallax) {
        this->ecs.kill_entity(e);
    }
    this->_parallax.clear();
    this->_parallax.push_back(this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxFirstbkg), (-0.070f * _resMult), sf::Vector2f(_resMult, _resMult), _resMult));
    this->_parallax.push_back(this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxSecondbkg), (-0.1f * _resMult), sf::Vector2f(_resMult, _resMult), _resMult));

    auto it = this->_screens.find(SCREEN_TYPE::MAIN_MENU);
    if (it != this->_screens.end()) {
        this->ecs.enableEntity(it->second);
    } else {
        this->_screens.emplace(EntityManager::SCREEN_TYPE::MAIN_MENU, this->_factory.createScreen(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::MenuScreen)));
    }

    this->_menuManager.enableMenu(MenuManager::MENU_TYPE::MAIN_MENU);
    this->handleMusic(this->ecs, EntityManager::MUSIC_TYPE::LOBBY, [&](ECS::components::MusicComponent &music) {
        music.playMusic();
    });
}

void Game::killGameEntity()
{
    this->_started = false;
    this->stopAllMusic(this->ecs);

    for (auto &e : this->_players) {
        this->ecs.kill_entity(e.second);
    }
    this->_players.clear();

    for (auto &e : this->_missiles) {
        this->ecs.kill_entity(e.second);
    }
    this->_missiles.clear();

    for (auto &e : this->_ennemies) {
        this->ecs.kill_entity(e.second);
    }
    this->_ennemies.clear();

    for (auto &e : this->_bonuses) {
        this->ecs.kill_entity(e.second);
    }
    this->_bonuses.clear();

    for (auto &e : this->_bombs) {
        this->ecs.kill_entity(e.second);
    }
    this->_bombs.clear();

    for (auto &e : this->_loadingBar) {
        this->ecs.kill_entity(e.second);
    }
    this->_loadingBar.clear();

    this->ecs.kill_entity(this->_scoreCoche);
    this->_scoreCoche = 0;

    for (auto &e : this->_textsEntity) {
        if (this->ecs.isEntityExist(e.second))
            this->ecs.kill_entity(e.second);
    }
    this->_textsEntity.clear();

    if (this->ecs.isEntityExist(_looser)) {
        this->ecs.kill_entity(_looser);
    }

    for (auto &e : this->_strobes) {
        this->ecs.disableEntity(e);
    }
}

void Game::refreshScreenSize()
{
    if (this->_realScreenSizeU.x != this->_window.getSize().x || this->_realScreenSizeU.y != this->_window.getSize().y) {
        this->_realScreenSize = {static_cast<float>(this->_window.getSize().x), static_cast<float>(this->_window.getSize().y)};
        this->_realScreenSizeU = {this->_window.getSize().x, this->_window.getSize().y};

        if (_realScreenSize.x / _realScreenSize.y <= 16.0f / 9.0f) {
            this->_screenSize = {this->_realScreenSize.x, this->_realScreenSize.x * ((1.0f / 16.0f) / (1.0f / 9.0f)) };
            unsigned int difHeight = this->_realScreenSize.y - this->_screenSize.y;
            difHeight /= 2;
            this->topLeftOffeset = {0, difHeight};
            _blackBandTopLeft = this->_factory.createBlackband(sf::IntRect(0, 0, this->_realScreenSize.x, difHeight), this->_manager.getTexture(Loader::Loader::BlackPixel));
            _blackBandBottomRight = this->_factory.createBlackband(sf::IntRect(0, this->_realScreenSize.y - difHeight, this->_realScreenSize.x, difHeight), this->_manager.getTexture(Loader::Loader::BlackPixel));
        } else {
            this->_screenSize = {this->_realScreenSize.y * ((1.0f / 9.0f) / (1.0f / 16.0f)), this->_realScreenSize.y};
            unsigned int difWidth = this->_realScreenSize.x - this->_screenSize.x;
            difWidth /= 2;
            this->topLeftOffeset = {difWidth, 0};
            _blackBandTopLeft = this->_factory.createBlackband(sf::IntRect(0, 0, difWidth, this->_realScreenSize.y), this->_manager.getTexture(Loader::Loader::BlackPixel));
            _blackBandBottomRight = this->_factory.createBlackband(sf::IntRect(this->_realScreenSize.x - difWidth, 0, difWidth, this->_realScreenSize.y), this->_manager.getTexture(Loader::Loader::BlackPixel));
        }
    }
}

void Game::initButtons()
{
    float tmpSizebutton = (this->_manager.getTexture(Loader::Loader::CreateRoomButton).get()->getSize().x / 2) * _resMult;

    this->_menuManager.createButton(MenuManager::BUTTON_TYPE::CREATE_GAME, this->_factory.createButton(300.0f + (tmpSizebutton / 2), 850.0f + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::CreateRoomButton), sf::Vector2f(_resMult + 0.3, _resMult + 0.3),
    [&](void) {
        this->_menuManager.disableMenu(MenuManager::MENU_TYPE::MAIN_MENU);
        this->_gameState = gameState::MATCHMAKING;

        Stream out;
        out << 8_uc << 0_uc;
        this->_net.send(out);
    }));

    this->_menuManager.createButton(MenuManager::BUTTON_TYPE::JOIN_GAME, this->_factory.createButton(450.0f + (tmpSizebutton), 850.0f + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::JoinRoomButton), sf::Vector2f(_resMult + 0.3, _resMult + 0.3),
    [&](void) {
        this->_menuManager.disableMenu(MenuManager::MENU_TYPE::MAIN_MENU);
        this->_gameState = gameState::MATCHMAKING;

        Stream out;
        out << 9_uc;
        this->_net.send(out);
    }
    ));

    tmpSizebutton = (this->_manager.getTexture(Loader::Loader::QuitButton).get()->getSize().x / 2) * _resMult;

    this->_menuManager.createButton(MenuManager::BUTTON_TYPE::EXIT_SYSTEM, this->_factory.createButton(this->_screenSize.x - (tmpSizebutton + 20), 10.0f + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::QuitButton), sf::Vector2f(_resMult + 0.1, _resMult + 0.1),
    [&](void) {
        this->_window.close();
    }
    ));

    tmpSizebutton = (this->_manager.getTexture(Loader::Loader::MatchListButton).get()->getSize().x / 2) * _resMult;
    float tmpSizebuttonY = (this->_manager.getTexture(Loader::Loader::MatchListButton).get()->getSize().y) * _resMult;

    entity_t newButton;

    for (size_t buttonNbr = 0; buttonNbr < 6; ++buttonNbr) {
        newButton = this->_factory.createButton(10.0f + (tmpSizebutton / 2), (340.0f + this->topLeftOffeset.y) + ((tmpSizebuttonY + 10) * buttonNbr), this->_manager.getTexture(Loader::Loader::MatchListButton), sf::Vector2f(_resMult, _resMult),
        [&, buttonNbr](void) {
            size_t nodePos = buttonNbr;
            std::tuple<int, entity_t, entity_t> foundTuple = this->_roomsData[nodePos];
            u_int roomId = std::get<0>(foundTuple);

            if (roomId == -1)
                return;

            this->_menuManager.disableMenu(MenuManager::MENU_TYPE::MAIN_MENU);
            this->_gameState = gameState::MATCHMAKING;

            for (auto &node : this->_roomsData) {
                std::tuple<int, entity_t, entity_t> foundTuple = node;
                if (std::get<0>(foundTuple) == -1)
                    continue;
                if (this->ecs.isEntityExist(std::get<1>(foundTuple)))
                    this->ecs.kill_entity(std::get<1>(foundTuple));
                if (this->ecs.isEntityExist(std::get<2>(foundTuple)))
                    this->ecs.kill_entity(std::get<2>(foundTuple));
                std::get<0>(foundTuple) = -1;
                node = foundTuple;
            }

            Stream out;
            out << 25_uc << roomId;
            this->_net.send(out);
        });
        this->_menuManager.createButton((MenuManager::BUTTON_TYPE)(MenuManager::BUTTON_TYPE::ROOM_LIST_0 + buttonNbr), newButton);
    }

    tmpSizebutton = (this->_manager.getTexture(Loader::Loader::LeaveGame).get()->getSize().x / 2) * _resMult;
    this->_menuManager.createButton(MenuManager::BUTTON_TYPE::LEAVE_GAME, this->_factory.createButton((this->_screenSize.x / 2) - (tmpSizebutton), (this->_screenSize.y) - 200, this->_manager.getTexture(Loader::Loader::LeaveGame), sf::Vector2f(_resMult + 0.8, _resMult + 0.8),
    [&](void) {
        Stream out;
        out << 24_uc;
        this->_net.send(out);
        this->killGameEntity();
        this->createMainMenuScene();
    }
    ));
}

void Game::initMenus()
{
    entity_t entity_mainMenu = this->ecs.spawn_entity();
    this->ecs.emplace_component<ECS::components::ControllableComponent>(entity_mainMenu, ECS::components::ControllableComponent{
        sf::Keyboard::Key::Up,
        sf::Keyboard::Key::Down,
        sf::Keyboard::Key::Left,
        sf::Keyboard::Key::Right,
        sf::Keyboard::Key::Enter
    });

    this->_menuManager.createMenu(MenuManager::MENU_TYPE::MAIN_MENU, entity_mainMenu, true, std::vector<MenuManager::BUTTON_TYPE>({
        MenuManager::BUTTON_TYPE::ROOM_LIST_0,
        MenuManager::BUTTON_TYPE::ROOM_LIST_1,
        MenuManager::BUTTON_TYPE::ROOM_LIST_2,
        MenuManager::BUTTON_TYPE::ROOM_LIST_3,
        MenuManager::BUTTON_TYPE::ROOM_LIST_4,
        MenuManager::BUTTON_TYPE::ROOM_LIST_5,
        MenuManager::BUTTON_TYPE::CREATE_GAME,
        MenuManager::BUTTON_TYPE::JOIN_GAME,
        MenuManager::BUTTON_TYPE::EXIT_SYSTEM
    }));

    entity_t entity_looseMenu = this->ecs.spawn_entity();
    this->ecs.emplace_component<ECS::components::ControllableComponent>(entity_looseMenu, ECS::components::ControllableComponent{
        sf::Keyboard::Key::Enter,
        sf::Keyboard::Key::Right
    });
    this->_menuManager.createMenu(MenuManager::MENU_TYPE::LOOSER_MENU, entity_looseMenu, false, std::vector<MenuManager::BUTTON_TYPE>({
        MenuManager::BUTTON_TYPE::LEAVE_GAME
    }));
}

void Game::update()
{
    Network::Packet packet;

    static const std::unordered_map<int, std::function<void(Network::Packet &)>> instructionsAllTime = {
        {10, [&](Network::Packet &packet) { Game::handleRoomJoin(packet);} },
        {11, [&](Network::Packet &packet) { Game::handleTimeoutMatchmaking(packet);} },
        {13, [&](Network::Packet &packet) { Game::handlePlayerJoinGame(packet);} },
        {14, [&](Network::Packet &packet) { Game::handlePlayerDisconnected(packet);} },
        {17, [&](Network::Packet &packet) { Game::handleGameEnd(packet);} },
        {23, [&](Network::Packet &packet) { Game::handleLatency(packet);} },
        {27, [&](Network::Packet &packet) { Game::handleListRooms(packet);} },
        {255, [&](Network::Packet &packet) { this->handleResend(packet); }},
    };

    static const std::unordered_map<int, std::function<void(Network::Packet &)>> instructionsWhenPlay = {
        {3, [&](Network::Packet &packet) { Game::handlePlayerPosition(packet);} },
        {4, [&](Network::Packet &packet) { this->handleMissilePosition(packet); }},
        {6, [&](Network::Packet &packet) { this->handlePlayerScore(packet); }},
        {7, [&](Network::Packet &packet) { this->handleEnnemiPosition(packet); }},
        {15, [&](Network::Packet &packet) { this->handleMissileDeath(packet); }},
        {16, [&](Network::Packet &packet) { this->handleEnnemiDeath(packet); }},
        {18, [&](Network::Packet &packet) { this->handlePlayerDeath(packet); }},
        {19, [&](Network::Packet &packet) { this->handlePlayerLife(packet); }},
        {20, [&](Network::Packet &packet) { this->handleMonsterLife(packet); }},
        {21, [&](Network::Packet &packet) { this->handleStrobes(packet); }},
        {22, [&](Network::Packet &packet) { this->handleChangeLevel(packet); }},
        {28, [&](Network::Packet &packet) { this->handleBonusPosition(packet); }},
        {29, [&](Network::Packet &packet) { this->handleBonusDestroyed(packet); }},
        {31, [&](Network::Packet &packet) { this->handleBombPosition(packet); }},
        {32, [&](Network::Packet &packet) { this->handleBombDestroyed(packet); }},
        {33, [&](Network::Packet &packet) { this->handleChatMessage(packet); }},
        {35, [&](Network::Packet &packet) { this->handlePodInfo(packet); }},
        {37, [&](Network::Packet &packet) { this->handleLaser(packet); }},
        {39, [&](Network::Packet &packet) { this->handleRay(packet); }},
        {40, [&](Network::Packet &packet) { this->handleBonus(packet); }},
    };

    while (this->_net.getQueueIn().tryPop(packet)) {

        auto it = instructionsAllTime.find(packet.getInstruction());
        if (it != instructionsAllTime.end()) {
            it->second(packet);
            continue;
        }

        if (this->_gameState == gameState::MENU)
            continue;

        it = instructionsWhenPlay.find(packet.getInstruction());
        if (it != instructionsWhenPlay.end()) {
            it->second(packet);
            continue;
        }
    }
    if (std::chrono::system_clock::now() - this->_lastPing > std::chrono::seconds(1)) {
        this->_lastPing = std::chrono::system_clock::now();
        Stream out;
        if (this->_gameState == gameState::MENU) {
            out << 26_uc;
            this->_net.send(out);
        }
        out.clear();
        out << 12_uc << static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(_lastPing.time_since_epoch()).count());
        this->_net.send(out);
    }

    if (this->_gameState == gameState::GAME) {
        float timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->_startGameTime).count() / 1000.0f;

        int minutes = static_cast<int>(timer) / 60;
        int seconds = static_cast<int>(timer) % 60;

        std::ostringstream ss;
        ss << "Timer: " << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;

        entity_t gameTimeText = this->getTextByType(game::EntityManager::TEXT_TYPE::GAME_TIME);
        if (gameTimeText == 0) {
            entity_t newEntity = this->_factory.createText(ss.str(), this->_manager.getFont(Loader::Loader::PressStart2P), this->_screenSize.x / 2 - (250 * this->_resMult), this->topLeftOffeset.y + 40, 20);
            this->_textsEntity.insert({game::EntityManager::TEXT_TYPE::GAME_TIME, newEntity});
        } else {
            this->_textsUpdate.insert(std::make_pair(gameTimeText, ss.str()));
        }
    }
}

void Game::sendMoveToServer()
{
    for (auto i = this->_entityEvents.begin(); i != this->_entityEvents.end(); ++i) {
        if ((this->_gameState == gameState::MENU || this->_gameState == gameState::ENDGAME)
            && (*i).getEntity() == this->_menuManager.getMenuEntity(MenuManager::MENU_TYPE::MAIN_MENU)) {
            if (((*i).getEvent() & ENTER)) {
                if (this->_menuManager.executeButtonInMenu(this->ecs) == true) {
                    entity_t soundEntity = this->_factory.createSound(client::getAssetPath("songs/effects/click_button.ogg"), 1000, true);
                    this->_sounds.emplace_back(soundEntity);
                }
            }
            if ((*i).getEvent() & DOWN) {
                this->_menuManager.nextButtonInMenu(MenuManager::MENU_TYPE::MAIN_MENU);
            }
            if ((*i).getEvent() & UP) {
                this->_menuManager.previousButtonInMenu(MenuManager::MENU_TYPE::MAIN_MENU);
            }
            continue;
        }

        if (this->_gameState == gameState::GAME && (*i).getEntity() == this->_playerEntity) {
            char move = (*i).getEvent() & (UP | DOWN | LEFT | RIGHT);
            if ((*i).getEvent() & move) {
                Stream out;
                out << 2_uc << static_cast<u_char>((*i).getEvent() & move) << 1_uc;
                _net.send(out);
            }
            if ((*i).getEvent() & SPACE && std::chrono::system_clock::now() - this->_lastPlayerFireTime > std::chrono::milliseconds(140)) {
                this->_lastPlayerFireTime = std::chrono::system_clock::now();
                Stream out;
                out << 5_uc;
                this->_net.send(out);

            }
            if ((*i).getEvent() & BOMB && std::chrono::system_clock::now() - this->_lastPlayerBombFireTime > std::chrono::milliseconds(900)) {
                this->_lastPlayerBombFireTime = std::chrono::system_clock::now();
                Stream out;
                out << 30_uc;
                this->_net.send(out);
            }
            if ((*i).getEvent() & LASER && std::chrono::system_clock::now() - this->_lastPlaerLaserFireTime > std::chrono::milliseconds(4900)) {
                this->_lastPlaerLaserFireTime = std::chrono::system_clock::now();
                Stream out;
                out << 36_uc;
                this->_net.send(out);
            }
            if ((*i).getEvent() & RAY && std::chrono::system_clock::now() - this->_lastPlaerRayFireTime > std::chrono::milliseconds(2900)) {
                this->_lastPlaerRayFireTime = std::chrono::system_clock::now();
                Stream out;
                out << 38_uc;
                this->_net.send(out);
            }
            continue;
        }
    }
    this->_entityEvents.clear();
}

void Game::handleChatInput(float deltaTime)
{
    float updateInterval = 30.0f;
    this->_timeSinceLastUpdate += deltaTime;

    if (this->_gameState != gameState::MATCHMAKING || this->_timeSinceLastUpdate < updateInterval) return;

    entity_t tchatText = this->getTextByType(EntityManager::TEXT_TYPE::TCHAT);
    if (this->ecs.isEntityExist(tchatText) == false) return;

    size_t inputSize = this->_keyboardInputs.size();
    if (inputSize < 1) return;
    for (auto &i : _keyboardInputs) {
        if (i == '\n' || i == '\r') {
                while (!this->_chatInput.empty() && std::isspace(this->_chatInput.back())) {
                this->_chatInput.pop_back();
            }
            if (!this->_chatInput.empty()) {
                this->sendChat(this->_chatInput);
                this->_chatInput.clear();
            }
            break;
        } else if (i == '\b') {
            if (!this->_chatInput.empty())
                this->_chatInput.pop_back();
        } else if (this->_chatInput.size() < 26) {
            this->_chatInput += i;
        }
    }

    this->_textsUpdate.insert(std::make_pair(tchatText, this->_chatInput));
    this->_keyboardInputs.clear();
    this->_timeSinceLastUpdate = 0.0f;
}

int Game::MainLoop()
{
    while (this->_window.isOpen()) {
        this->_fbr->refresh();
        this->_fbr->fadeout();
        this->GenParticles();

        this->refreshScreenSize();
        long currentTime = NOW;
        float deltaTime = (currentTime - this->_lastTime) / 1.0f;
        this->_lastTime = currentTime;

        this->handleChatInput(deltaTime);
        this->update();

        // ALL SYSTEMS CALL HERE
        ECS::systems::ControllableSystem().update(this->ecs, this->_entityEvents, this->_keyboardInputs, this->_window, this->eventMemory);
        ECS::systems::PositionSystem().update(this->ecs, this->topLeftOffeset);
        ECS::systems::AnimationSystem().update(this->ecs, deltaTime);
        ECS::systems::AnimationOneTimeSystem().update(this->ecs, deltaTime);
        ECS::systems::ParallaxSystem().update(this->ecs, deltaTime, this->topLeftOffeset);
        ECS::systems::MovableSystem().update(this->ecs, this->_entityPositions, this->topLeftOffeset);
        ECS::systems::ScaleSystem().update(this->ecs);
        ECS::systems::TextSystem().update(this->ecs, this->_textsUpdate);
        ECS::systems::SoundSystem().update(this->ecs, this->_sounds);
        this->_window.clear();

        // DRAW SYSTEM CALL HERE
        ECS::systems::DrawSystem().update(this->ecs, this->_window);
        this->_window.display();
        this->sendMoveToServer();
    }
    this->_net.setClosed(true);
    Stream out;
    out << 12_uc << static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(_lastPing.time_since_epoch()).count());
    this->_net.send(out);
    return 0;
}

// UPDATE HANDLE FUNCTIONS

void Game::handlePlayerPosition(Network::Packet &packet)
{
    unsigned int id;
    short x;
    short y;
    packet >> id >> x >> y;
    x *= this->_resMult;
    y *= this->_resMult;

    std::pair<entity_t, unsigned char> tmp = getPodEntityFromId(id);

    if (tmp.first != 0) {
        if (tmp.second == 1)
            this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(tmp.first, x + 240 * _resMult, y + 41 * _resMult));
        if (tmp.second == 2)
            this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(tmp.first, x + 240 * _resMult, y + 33 * _resMult));
        if (tmp.second == 3)
            this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(tmp.first, x + 240 * _resMult, y + 17 * _resMult));
    }
    this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(this->getPlayerEntityFromId(id), x, y));
}

void Game::handleMissilePosition(Network::Packet &packet)
{
    unsigned int id;
    unsigned char type;
    short x;
    short y;
    packet >> id >> type >> x >> y;
    x *= this->_resMult;
    y *= this->_resMult;

    entity_t res = getMissileEntityFromId(id);

    if (res == 0) {
        switch ((int)type) {
            case MISSILE_TYPE::PLAYER_ONE: {
                    entity_t newEntity = this->_factory.createMissile(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Missile));
                    this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                    this->_missiles.push_back(std::make_pair(id, newEntity));
                    entity_t soundEntity = this->_factory.createSound(client::getAssetPath("songs/effects/piou.ogg"), 1000, true);
                    this->_sounds.emplace_back(soundEntity);
                }
                break;
            case MISSILE_TYPE::LITTLE_MONSTER: {
                    entity_t newEntity = this->_factory.createMissile(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Missile));
                    this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                    this->_missiles.push_back(std::make_pair(id, newEntity));
                }
                break;
            case MISSILE_TYPE::ORANGE_MISSILE: {
                    entity_t newEntity = this->_factory.createMissileAnnimated(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::OrangeMissile), this->_resMult, 4);
                    this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                    this->_missiles.push_back(std::make_pair(id, newEntity));
                }
                break;
            case MISSILE_TYPE::PURPLE_MISSILE: {
                    entity_t newEntity = this->_factory.createMissileAnnimated(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::PurpleMissile), this->_resMult, 4);
                    this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                    this->_missiles.push_back(std::make_pair(id, newEntity));
                }
                break;
            case MISSILE_TYPE::GREEN_MISSILE: {
                    entity_t newEntity = this->_factory.createMissileAnnimated(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::GreenMissile), this->_resMult, 4);
                    this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                    this->_missiles.push_back(std::make_pair(id, newEntity));
                }
                break;
            case MISSILE_TYPE::PLAYER_TWO: {
                    entity_t newEntity = this->_factory.createMissile(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::MissileRed));
                    this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                    this->_missiles.push_back(std::make_pair(id, newEntity));
                    entity_t soundEntity = this->_factory.createSound(client::getAssetPath("songs/effects/piou.ogg"), 1000, true);
                    this->_sounds.emplace_back(soundEntity);
                }
                break;
            case MISSILE_TYPE::PLAYER_THREE: {
                    entity_t newEntity = this->_factory.createMissile(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::MissileRed));
                    this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                    this->_missiles.push_back(std::make_pair(id, newEntity));
                    entity_t soundEntity = this->_factory.createSound(client::getAssetPath("songs/effects/piou.ogg"), 1000, true);
                    this->_sounds.emplace_back(soundEntity);
                }
                break;
            case MISSILE_TYPE::FIRE_BALL: {
                    entity_t newEntity = this->_factory.createMissile(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::FireBall));
                    this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                    this->_missiles.push_back(std::make_pair(id, newEntity));
                }
                break;
            default:
                break;
        }
    } else {
        this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(res, x, y));
    }
}

void Game::handlePlayerScore(Network::Packet &packet)
{
    unsigned int score = 0;
    packet >> score;
    std::ostringstream ss;
    ss <<  "Score: " << std::fixed << std::setprecision(1) << score;

    entity_t scoreText = this->getTextByType(EntityManager::TEXT_TYPE::SCORE);
    if (scoreText == 0) {
        entity_t newEntity = this->_factory.createText(ss.str(), this->_manager.getFont(Loader::Loader::PressStart2P), this->_screenSize.x / 2 - (250 * this->_resMult), 10, 20);
        this->_textsEntity.insert({EntityManager::TEXT_TYPE::SCORE, newEntity});
    } else {
        this->_textsUpdate.insert(std::make_pair(scoreText, ss.str()));
    }
}

void Game::handleEnnemiPosition(Network::Packet &packet)
{
    unsigned int id;
    unsigned char type;
    short x;
    short y;
    packet >> id >> type >> x >> y;
    x *= this->_resMult;
    y *= this->_resMult;

    entity_t res = getEnnemiEntityFromId(id);
    if (res == 0) {
        switch ((int)type) {
            case 2: {
                    entity_t newEntity = this->_factory.createEnnemiFrames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Monster2), this->_resMult, 4);
                    this->_ennemies.push_back(std::make_pair(id, newEntity));
                }
                break;
            case 3: {
                    entity_t newEntity = this->_factory.createEnnemiFrames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Monster3), this->_resMult, 4);
                    this->_ennemies.push_back(std::make_pair(id, newEntity));
                }
                break;
            case 4: {
                    entity_t newEntity = this->_factory.createEnnemiFrames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Monster4), this->_resMult, 4);
                    this->_ennemies.push_back(std::make_pair(id, newEntity));
                }
                break;
            case 5: {
                    entity_t newEntity = this->_factory.createEnnemiFrames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Monster4), this->_resMult, 4);
                    this->_ennemies.push_back(std::make_pair(id, newEntity));
                }
            case 6: {
                    entity_t newEntity = this->_factory.createEnnemiFrames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Boss1), this->_resMult, 4);
                    this->_ennemies.push_back(std::make_pair(id, newEntity));
                }
                break;
            case 7: {
                    entity_t newEntity = this->_factory.createEnnemiFrames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Boss2), this->_resMult, 4);
                    this->_ennemies.push_back(std::make_pair(id, newEntity));
                }
                break;
            case 8: {
                    entity_t newEntity = this->_factory.createEnnemiFrames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Boss3), this->_resMult, 4);
                    this->_ennemies.push_back(std::make_pair(id, newEntity));
                }
                break;
            case 9: {
                    entity_t newEntity = this->_factory.createEnnemiFrames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Boss4), this->_resMult, 4);
                    this->_ennemies.push_back(std::make_pair(id, newEntity));
                }
                break;
            case 10: {
                    entity_t newEntity = this->_factory.createEnnemiFrames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Boss5), this->_resMult, 4);
                    this->_ennemies.push_back(std::make_pair(id, newEntity));
                }
                break;
            case 11: {
                    entity_t newEntity = this->_factory.createEnnemiFrames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Boss6), this->_resMult, 4);
                    this->_ennemies.push_back(std::make_pair(id, newEntity));
                }
                break;
            default:
                break;
        }
    } else {
        this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(res, x, y));
    }
}

void Game::handleRoomJoin(Network::Packet &packet)
{
    packet >> this->_roomId >> this->_playerId;

    std::shared_ptr<sf::Texture> texture = nullptr;

    switch (this->_playerId) {
        case 1:
            texture = this->_manager.getTexture(Loader::Loader::Player_move1);
            break;
        case 2:
            texture = this->_manager.getTexture(Loader::Loader::Player_move2);
            break;
        case 3:
            texture = this->_manager.getTexture(Loader::Loader::Player_move3);
            break;
        case 4:
            texture = this->_manager.getTexture(Loader::Loader::Player_move4);
            break;
        default:
            texture = this->_manager.getTexture(Loader::Loader::Player_move1);
            break;
    }
    entity_t newEntity = this->_factory.createPlayer(0, 0, texture);
    this->_players.push_back(std::make_pair(this->_playerId, newEntity));
    this->_playerEntity = newEntity;
    this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});

    this->_scoreCoche = this->_factory.createScoreCoche((((this->_window.getSize().x / 2) - ((660.0 * this->_resMult) / 2))), this->topLeftOffeset.y , this->_manager.getTexture(Loader::Loader::ScoreCoche), this->_resMult);

    if (this->_loadingBar.find(EntityManager::LOADINGBAR_TYPE::PLAYER_LIFE) == this->_loadingBar.end()) {
        entity_t playerLifeBar = this->_factory.createLoadingBar(this->topLeftOffeset.x + 10.0f, this->topLeftOffeset.y + this->_screenSize.y - (100.0f * this->_resMult), this->_manager.getTexture(Loader::Loader::playerLifeOutline), this->_manager.getTexture(Loader::Loader::playerLifeContent), this->_resMult);
        this->_loadingBar.insert(std::make_pair(EntityManager::LOADINGBAR_TYPE::PLAYER_LIFE, playerLifeBar));
    }
    this->_chatInput.clear();
    auto it = this->_screens.find(SCREEN_TYPE::MAIN_MENU);
    if (it != this->_screens.end()) {
        this->ecs.disableEntity(it->second);
    }

    for (auto &node : this->_roomsData) {
        std::tuple<int, entity_t, entity_t> foundTuple = node;
        if (std::get<0>(foundTuple) == -1)
            continue;
        if (this->ecs.isEntityExist(std::get<1>(foundTuple)))
            this->ecs.kill_entity(std::get<1>(foundTuple));
        if (this->ecs.isEntityExist(std::get<2>(foundTuple)))
            this->ecs.kill_entity(std::get<2>(foundTuple));
        std::get<0>(foundTuple) = -1;
        node = foundTuple;
    }

    this->stopAllMusic(this->ecs);
    this->handleMusic(this->ecs, EntityManager::MUSIC_TYPE::MATCHMAKING, [&](ECS::components::MusicComponent &music) {
        music.playMusic();
    });

    sf::Vector2u rectSize = this->_manager.getTexture(Loader::Loader::ChatBox).get()->getSize();

    auto it3 = this->_screens.find(SCREEN_TYPE::CHAT_BOX);
    if (it3 == this->_screens.end()) {
        entity_t newE = this->_factory.createScreen(this->_screenSize.x - (rectSize.x * (this->_resMult + 0.1)), (this->_screenSize.y / 2) - ((rectSize.y * (this->_resMult + 0.1)) / 2), this->_manager.getTexture(Loader::Loader::ChatBox), sf::Vector2f(this->_resMult + 0.1, this->_resMult + 0.1));
        this->_screens.emplace(EntityManager::SCREEN_TYPE::CHAT_BOX, newE);
    } else {
        this->ecs.enableEntity(it3->second);
    }

    entity_t newEntityTchat = this->_factory.createText("", this->_manager.getFont(Loader::Loader::PressStart2P), (this->_screenSize.x - (rectSize.x * (this->_resMult + 0.1))) + 32, (this->_screenSize.y / 2) + 305, 14);
    this->_textsEntity.insert({game::EntityManager::TEXT_TYPE::TCHAT, newEntityTchat});
}

void Game::handleTimeoutMatchmaking(Network::Packet &packet)
{
    packet >> this->_startTimeLeft >> this->_started >> this->currentSong;

    entity_t timerText = this->getTextByType(game::EntityManager::TEXT_TYPE::TIMER);
    if (this->_started == true) {
        auto it = this->_screens.find(SCREEN_TYPE::CHAT_BOX);
        if (it != this->_screens.end()) {
            this->ecs.disableEntity(it->second);
        }

        for (auto &e : this->_textChat) {
            this->ecs.kill_entity(e);
        }
        this->_textChat.clear();

        auto it2 = this->_textsEntity.find(EntityManager::TEXT_TYPE::TCHAT);
        if (it2 != this->_textsEntity.end()) {
            this->ecs.kill_entity(it2->second);
            this->_textsEntity.erase(it2);
        }
        this->_chatInput.clear();

        entity_t soundEntity = this->_factory.createSound(client::getAssetPath("songs/effects/good_luck.ogg"), 1000, true);
        this->_sounds.emplace_back(soundEntity);

        if (timerText != 0)
            this->ecs.kill_entity(timerText);
        this->_gameState = gameState::GAME;
        this->_startGameTime = std::chrono::system_clock::now();

        entity_t newEntity = this->_factory.createText("Score: 0", this->_manager.getFont(Loader::Loader::PressStart2P), this->_screenSize.x / 2 - (250 * this->_resMult), this->topLeftOffeset.y + 10, 20);
        this->_textsEntity.insert({EntityManager::TEXT_TYPE::SCORE, newEntity});
        this->stopAllMusic(this->ecs);
        this->handleMusic(this->ecs, static_cast<EntityManager::MUSIC_TYPE>(this->currentSong), [](ECS::components::MusicComponent &music) {
            music.playMusic();
        });
        for (auto &parallax : this->_parallax ) {
            this->ecs.modify_component<ECS::components::ParallaxComponent>(parallax, [](ECS::components::ParallaxComponent &comp) {
                comp.setScrollSpeed(comp.getScrollSpeed() * 4.0f);
            });
        }
    } else {
        float timer = this->_startTimeLeft / 1000.0f;
        std::ostringstream ss;
        ss << "Match making: " << std::fixed << std::setprecision(1) << timer;
        if (timerText == 0) {
            entity_t newEntity = this->_factory.createText(ss.str(), this->_manager.getFont(Loader::Loader::PressStart2P), this->_screenSize.x / 2 - 135, this->topLeftOffeset.y + 25, 16);
            this->_textsEntity.insert({game::EntityManager::TEXT_TYPE::TIMER, newEntity});
        } else {
            this->_textsUpdate.insert(std::make_pair(timerText, ss.str()));
        }
    }
}

void Game::handlePlayerJoinGame(Network::Packet &packet)
{
    unsigned int id;
    packet >> id;
    std::shared_ptr<sf::Texture> texture = nullptr;

    switch (id) {
        case 1:
            texture = this->_manager.getTexture(Loader::Loader::Player_move1);
            break;
        case 2:
            texture = this->_manager.getTexture(Loader::Loader::Player_move2);
            break;
        case 3:
            texture = this->_manager.getTexture(Loader::Loader::Player_move3);
            break;
        case 4:
            texture = this->_manager.getTexture(Loader::Loader::Player_move4);
            break;
        default:
            texture = this->_manager.getTexture(Loader::Loader::Player_move1);
            break;
    }

    if (texture != nullptr) {
        entity_t newEntity = this->_factory.createPlayer(0, 0, texture);
        this->_players.push_back(std::make_pair(id, newEntity));
        this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});

    }
}

void Game::handlePlayerDisconnected(Network::Packet &packet)
{
    unsigned int id;
    packet >> id;

    entity_t res = getPlayerEntityFromId(id);
    if (res != 0) {
        this->ecs.kill_entity(res);

        this->_entityPositions.erase(std::remove_if(this->_entityPositions.begin(), this->_entityPositions.end(), [id](ECS::systems::MovableSystem::EntityPos const &pair) {
            return pair.getEntity() == id;
        }), this->_entityPositions.end());

        this->_players.erase(std::remove_if(this->_players.begin(), this->_players.end(), [id](std::pair<unsigned int, entity_t> const &pair) {
            return pair.first == id;
        }), this->_players.end());
    }
}

void Game::handleMissileDeath(Network::Packet &packet)
{
    unsigned int id;
    unsigned char type;
    short x;
    short y;
    packet >> id >> type >> x >> y;
    x *= this->_resMult;
    y *= this->_resMult;

    entity_t res = getMissileEntityFromId(id);
    if (res != 0) {
        this->ecs.kill_entity(res);

        this->_entityPositions.erase(std::remove_if(this->_entityPositions.begin(), this->_entityPositions.end(), [id](ECS::systems::MovableSystem::EntityPos const &pair) {
            return pair.getEntity() == id;
        }), this->_entityPositions.end());

        this->_missiles.erase(std::remove_if(this->_missiles.begin(), this->_missiles.end(), [id](std::pair<unsigned int, entity_t> const &pair) {
            return pair.first == id;
        }), this->_missiles.end());
    }
}

void Game::handleEnnemiDeath(Network::Packet &packet)
{
    unsigned int id;
    packet >> id;

    entity_t res = getEnnemiEntityFromId(id);
    if (res != 0) {

        auto sprite  = this->ecs.getComponent<ECS::components::SpriteComponent>(res);
        CreateParticle(sprite.getSprite().getGlobalBounds() , sf::Color::White, 150);

        this->ecs.kill_entity(res);
        this->_entityPositions.erase(std::remove_if(this->_entityPositions.begin(), this->_entityPositions.end(), [id](ECS::systems::MovableSystem::EntityPos const &pair) {
            return pair.getEntity() == id;
        }), this->_entityPositions.end());

        this->_ennemies.erase(std::remove_if(this->_ennemies.begin(), this->_ennemies.end(), [id](std::pair<unsigned int, entity_t> const &pair) {
            return pair.first == id;
        }), this->_ennemies.end());
    }
}

void Game::handleGameEnd(Network::Packet &packet)
{
    unsigned char type;
    packet >> type;
    this->_gameState = gameState::ENDGAME;
}

void Game::handlePlayerDeath(Network::Packet &packet)
{
    unsigned int id;
    packet >> id;

    entity_t res = getPlayerEntityFromId(id);
    if (res == this->_playerEntity) {
        entity_t soundEntity = this->_factory.createSound(client::getAssetPath("songs/effects/your_dead.ogg"), 1000, true);
        this->_sounds.emplace_back(soundEntity);

        this->_looser = this->_factory.createScreen(this->topLeftOffeset.x, this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::LooserScreen));
        this->ecs.emplace_component<ECS::components::ScaleComponent>(this->_looser, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
        this->_menuManager.enableMenu(MenuManager::MENU_TYPE::LOOSER_MENU);
        this->_gameState = gameState::ENDGAME;
    } else {
        entity_t soundEntity = this->_factory.createSound(client::getAssetPath("songs/effects/tango_down.ogg"), 1000, true);
        this->_sounds.emplace_back(soundEntity);
    }

    if (res != 0) {

        this->ecs.kill_entity(res);

        this->_entityPositions.erase(std::remove_if(this->_entityPositions.begin(), this->_entityPositions.end(), [id](ECS::systems::MovableSystem::EntityPos const &pair) {
            return pair.getEntity() == id;
        }), this->_entityPositions.end());

        this->_players.erase(std::remove_if(this->_players.begin(), this->_players.end(), [id](std::pair<unsigned int, entity_t> const &pair) {
            return pair.first == id;
        }), this->_players.end());

        entity_t pod = getPodEntityFromId(id).first;
        if (pod != 0) {
            for(auto i = _pods.begin(); i != _pods.end(); ++i) {
                if (std::get<0>(*i) == id) {
                    _pods.erase(i);
                    break;
                }
            }
            this->ecs.kill_entity(pod);
        }
    }
}

void Game::handlePlayerLife(Network::Packet &packet)
{
    int life;
    packet >> life;

    if (_loadingBar.find(EntityManager::LOADINGBAR_TYPE::PLAYER_LIFE) == _loadingBar.end())
        return;
    entity_t loadingPlayerlifeBar = _loadingBar[EntityManager::LOADINGBAR_TYPE::PLAYER_LIFE];

    if (this->ecs.hasComponent<ECS::components::LoadingBarComponent>(loadingPlayerlifeBar) == false) {
        auto &spriteComp = this->ecs.getComponent<ECS::components::SpriteComponent>(loadingPlayerlifeBar);
        this->ecs.emplace_component<ECS::components::LoadingBarComponent>(loadingPlayerlifeBar, ECS::components::LoadingBarComponent{static_cast<float>(life), static_cast<float>(spriteComp.getTextureRect().getSize().x)});
    }

    auto &barComp = this->ecs.getComponent<ECS::components::LoadingBarComponent>(loadingPlayerlifeBar);
    float newWidth = barComp.calculate(life);

    this->ecs.modify_component<ECS::components::RectangleShapeComponent>(this->_loadingBar[EntityManager::LOADINGBAR_TYPE::PLAYER_LIFE], [newWidth](ECS::components::RectangleShapeComponent &comp) {
        comp.setSize(sf::Vector2f(newWidth, comp.getSize().y));
    });
}

void Game::handleMonsterLife(Network::Packet &packet)
{
    u_int id;
    int life;
    packet >> id >> life;

    entity_t res = getEnnemiEntityFromId(id);
    if (res == 0)
        return;
}

void Game::handleStrobes(Network::Packet &packet)
{
    unsigned char color;
    unsigned char onOff;
    packet >> color >> onOff;
    if (onOff) {
        float x = this->topLeftOffeset.x;
        float y = this->topLeftOffeset.y;
        this->ecs.enableEntity(this->_strobes[color - 1]);
        this->ecs.modify_component<ECS::components::PositionComponent>(this->_strobes[color - 1], [x, y](ECS::components::PositionComponent &comp) {
            comp.setX(x);
            comp.setY(y);
        });
    } else {
        float x = this->_screenSize.x;
        float y = this->_screenSize.y;
        this->ecs.disableEntity(this->_strobes[color - 1]);
    }
}

void Game::handleResend(Network::Packet &packet)
{
    u_short nbr;
    packet >> nbr;
    _net.resend(nbr);
}

void Game::handleChangeLevel(Network::Packet &packet)
{
    unsigned int timeout = packet.getData().getDataUInt();
    unsigned char song = packet.getData().getDataUChar();
    unsigned char started = packet.getData().getDataUChar();
    unsigned int fadeOutTime = 7000;

    if (!started) {
        this->handleMusic(this->ecs, static_cast<EntityManager::MUSIC_TYPE>(this->currentSong), [timeout, fadeOutTime](ECS::components::MusicComponent &music) {
            if (timeout <= fadeOutTime) {
                int volume = SOUND_VOLUME * timeout / fadeOutTime;
                volume = (volume * -1) + SOUND_VOLUME;
                music.setVolume(volume);
            } else
                music.setVolume(0);
        });
    }
    if (started) {
        this->handleMusic(this->ecs, static_cast<EntityManager::MUSIC_TYPE>(this->currentSong), [timeout](ECS::components::MusicComponent &music) {
                music.stopmusic();
        });
        if (this->currentSong != song)
            this->currentSong = song;
        this->handleMusic(this->ecs, static_cast<EntityManager::MUSIC_TYPE>(this->currentSong), [](ECS::components::MusicComponent &music) {
            music.setVolume(100);
            music.playMusic();
        });

    }
}

void Game::handleLatency(Network::Packet &packet)
{
    u_short timeMS;
    packet >> timeMS;
    std::ostringstream ss;
    ss <<  "Ping: " << std::fixed << std::setprecision(1) << timeMS;

    entity_t pingText = this->getTextByType(game::EntityManager::TEXT_TYPE::PING);
    if (pingText == 0) {
        entity_t newEntity = this->_factory.createText(ss.str(), this->_manager.getFont(Loader::Loader::PressStart2P), this->_screenSize.x - 150, this->_screenSize.y - 30, 14);
        this->_textsEntity.insert({game::EntityManager::TEXT_TYPE::PING, newEntity});
    } else {
        this->_textsUpdate.insert(std::make_pair(pingText, ss.str()));
    }
}

int Game::searchRoomId(int roomId)
{
    int roomPosition = -1;

    for (int i = 0; i < _roomsData.size(); i++) {
        if (std::get<0>(_roomsData[i]) == roomId) {
            roomPosition = i;
            break;
        }
    }
    return roomPosition;
}

void Game::handleListRooms(Network::Packet &packet)
{
    u_int roomId;
    u_char nbrPlayers;
    u_char maxPlayers;
    bool joinable;
    packet >> roomId >> nbrPlayers >> maxPlayers >> joinable;

    int searchRoom = this->searchRoomId(roomId);
    if (searchRoom != -1) { // room not exist
        if (joinable == false) { // room is off
            std::tuple<int, entity_t, entity_t> foundTuple = this->_roomsData[searchRoom];

            if (this->ecs.isEntityExist(std::get<1>(foundTuple)))
                    this->ecs.kill_entity(std::get<1>(foundTuple));
            if (this->ecs.isEntityExist(std::get<2>(foundTuple)))
                this->ecs.kill_entity(std::get<2>(foundTuple));

            std::get<0>(foundTuple) = -1;
            this->_roomsData[searchRoom] = foundTuple;
        } else { // update room
            std::tuple<int, entity_t, entity_t> foundTuple = this->_roomsData[searchRoom];
            entity_t text2 = std::get<2>(foundTuple);

            this->ecs.modify_component<ECS::components::TextComponent>(text2, [nbrPlayers, maxPlayers](ECS::components::TextComponent &comp) {
                comp.setString(std::to_string(nbrPlayers) + "/" + std::to_string(maxPlayers));
            });
        }
    } else if (joinable == true) { // setup room data

        int firstEmptyNode = -1;
        auto it = _roomsData.begin();

        for (; it != _roomsData.end(); ++it) {
            if (std::get<0>(*it) == -1) {
                break;
            }
        }

        if (it != _roomsData.end()) {
            std::tuple<int, entity_t, entity_t> foundTuple = *it;
            int &foundInt = std::get<0>(foundTuple);
            entity_t &text1 = std::get<1>(foundTuple);
            entity_t &text2 = std::get<2>(foundTuple);

            foundInt = static_cast<int>(roomId);

            int nodePos = std::distance(_roomsData.begin(), it);
            sf::Vector2u buttonSize = this->_manager.getTexture(Loader::Loader::MatchListButton).get()->getSize();
            float sizeButtonX = (buttonSize.x / 2) * _resMult;
            float sizeButtonY = (buttonSize.y) * _resMult;

            float xPos = 90.0f + (sizeButtonX / 2);
            float yPos = (363.0f + this->topLeftOffeset.y) + ((sizeButtonY + 10) * nodePos);

            std::string text1Str = "Room number " + std::to_string(foundInt);
            text1 = this->_factory.createText(text1Str, this->_manager.getFont(Loader::Loader::PressStart2P), xPos, yPos, 16);

            std::string text2Str = std::to_string(nbrPlayers) + "/" + std::to_string(maxPlayers);
            text2 = this->_factory.createText(text2Str, this->_manager.getFont(Loader::Loader::PressStart2P), xPos + 320, yPos, 16);

            this->_roomsData[nodePos] = foundTuple;
        }
    }
}

void Game::handleBonusPosition(Network::Packet &packet)
{
    unsigned int id = packet.getData().getDataUInt();
    unsigned char type = packet.getData().getDataUChar();
    unsigned short x = packet.getData().getDataUShort();
    unsigned short y = packet.getData().getDataUShort();

    entity_t bonus = getBonusEntityFromId(id);
    if (bonus == 0) {
        bonus = this->_factory.createBonus(this->_manager.getTexture(Loader::Loader::Bonus), x * this->_resMult, y * this->_resMult, this->_resMult);
        this->_bonuses.push_back(std::make_pair(id, bonus));
    } else {
        this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(bonus, x * this->_resMult, y * this->_resMult));
    }
}

void Game::handleBonusDestroyed(Network::Packet &packet)
{
    unsigned int id = packet.getData().getDataUInt();

    entity_t entity = getBonusEntityFromId(id);
    if (entity != 0) {
        auto &c = this->ecs.getComponent<ECS::components::PositionComponent>(entity);
        if (c.getX() > 10) {
            entity_t soundEntity = this->_factory.createSound(client::getAssetPath("songs/effects/tracket.ogg"), 1000, true);
            this->_sounds.emplace_back(soundEntity);
        }

        this->ecs.kill_entity(entity);

        this->_entityPositions.erase(std::remove_if(this->_entityPositions.begin(), this->_entityPositions.end(), [id](ECS::systems::MovableSystem::EntityPos const &pair) {
            return pair.getEntity() == id;
        }), this->_entityPositions.end());

        this->_bonuses.erase(std::remove_if(this->_bonuses.begin(), this->_bonuses.end(), [id](std::pair<unsigned int, entity_t> const &pair) {
            return pair.first == id;
        }), this->_bonuses.end());
    }
}

void Game::handleBombPosition(Network::Packet &packet)

{
    unsigned int id = packet.getData().getDataUInt();
    unsigned short x = packet.getData().getDataUShort();
    unsigned short y = packet.getData().getDataUShort();

    entity_t bomb = getBombEntityFromId(id);
    if (bomb == 0) {
        bomb = this->_factory.createBomb(this->_manager.getTexture(Loader::Loader::Bomb), x * this->_resMult, y * this->_resMult, this->_resMult);
        this->_bombs.push_back(std::make_pair(id, bomb));
    } else {
        this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(bomb, x * this->_resMult, y * this->_resMult));
    }
}

void Game::handleBombDestroyed(Network::Packet &packet)
{
    unsigned int id = packet.getData().getDataUInt();

    entity_t entity = getBombEntityFromId(id);
    if (entity != 0) {

        this->_entityPositions.erase(std::remove_if(this->_entityPositions.begin(), this->_entityPositions.end(), [id](ECS::systems::MovableSystem::EntityPos const &pair) {
            return pair.getEntity() == id;
        }), this->_entityPositions.end());

        this->_bombs.erase(std::remove_if(this->_bombs.begin(), this->_bombs.end(), [id](std::pair<unsigned int, entity_t> const &pair) {
            return pair.first == id;
        }), this->_bombs.end());

        auto &c = this->ecs.getComponent<ECS::components::PositionComponent>(entity);
        if (c.getY() < this->_realScreenSize.y + 10) { // TO SEE
            entity_t soundEntity = this->_factory.createSound(client::getAssetPath("songs/effects/explose_pod.ogg"), 1000, true);
            this->_sounds.emplace_back(soundEntity);
        }

        this->ecs.modify_component<ECS::components::PositionComponent>(entity, [this](ECS::components::PositionComponent &comp) {
            this->_factory.createExplosion(this->_manager.getTexture(Loader::Loader::Explosion1),  comp.getX() - 240 * this->_resMult, comp.getY() - 241 * this->_resMult, this->_resMult);
        });


        this->ecs.kill_entity(entity);
    }
}

void Game::handleChatMessage(Network::Packet &packet)
{
    u_int playerId;
    std::string msg;
    char tmp;
    packet >> playerId;

    for (int i = 0; i < 1000; ++i) {
        packet >> tmp;
        if (tmp == 0)
            break;
        msg += tmp;
    }

    msg = std::to_string(playerId) + " > " + msg;

    sf::Vector2u rectSize = this->_manager.getTexture(Loader::Loader::ChatBox).get()->getSize();
    std::size_t vectorSize = this->_textChat.size();

    if (vectorSize > 16) {
        if (this->ecs.isEntityExist(this->_textChat.front()))
            this->ecs.kill_entity(this->_textChat.front());
        this->_textChat.erase(this->_textChat.begin());

        for (std::size_t i = 0; i < vectorSize - 1; i++) {
            this->ecs.modify_component<ECS::components::PositionComponent>(this->_textChat[i], [this, rectSize, vectorSize, i](ECS::components::PositionComponent &comp) {
                comp.setY(300 + i * 30);
            });
        }
    }
    vectorSize = this->_textChat.size();

    entity_t newEntity = this->_factory.createText(msg, this->_manager.getFont(Loader::Loader::PressStart2P), (this->_screenSize.x - (rectSize.x * (this->_resMult + 0.1))) + 32, (300 + (vectorSize * 30)), 14);
    this->_textChat.push_back(newEntity);
}

void Game::sendChat(std::string const &msg)
{
    Stream out;
    out << 34_uc;

    for (auto i = msg.begin(); i != msg.end(); i++) {
        out << *i;
    }

    for (std::size_t i = msg.size(); i < 1000; i++) {
        out << 0_c;
    }
    this->_net.send(out);
}


void Game::handlePodInfo(Network::Packet &packet)
{
    unsigned int userId = packet.getData().getDataUInt();
    unsigned char level = packet.getData().getDataUChar();
    unsigned char front = packet.getData().getDataUChar();


    std::pair<entity_t, unsigned char> tmp = getPodEntityFromId(userId);

    entity_t entity = tmp.first;
    if (entity == 0) {
        entity = this->_factory.createPod(this->_manager.getTexture(Loader::Loader::Pod1), this->_resMult);
        std::tuple<size_t, entity_t, unsigned char> newPod(static_cast<size_t>(userId), entity, front);
        this->_pods.push_back(newPod);
    } else {
        for(auto i = _pods.begin(); i != _pods.end(); ++i) {
            if (std::get<0>(*i) == userId) {
                std::get<2>(*i) = level;
                break;
            }
        }
    }
    this->ecs.modify_component<ECS::components::PositionComponent>(getPlayerEntityFromId(userId), [this, entity, level](ECS::components::PositionComponent &comp) {
        if (level == 1)
            this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(entity, comp.getX() + 240 * this->_resMult, comp.getY() + 41 * this->_resMult));
        if (level == 2) {
            const std::shared_ptr<sf::Texture> &texture = this->_manager.getTexture(Loader::Loader::Pod2);
            this->ecs.emplace_component<ECS::components::SpriteComponent>(entity, ECS::components::SpriteComponent{texture});
            this->ecs.emplace_component<ECS::components::TextureRectComponent>(entity, ECS::components::TextureRectComponent{0, 0, (int)texture->getSize().x, 64, 6, 150.0f});
            this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(entity, comp.getX() + 240 * this->_resMult, comp.getY() + 33 * this->_resMult));
        }
        if (level == 3) {
            const std::shared_ptr<sf::Texture> &texture = this->_manager.getTexture(Loader::Loader::Pod3);
            this->ecs.emplace_component<ECS::components::SpriteComponent>(entity, ECS::components::SpriteComponent{texture});
            this->ecs.emplace_component<ECS::components::TextureRectComponent>(entity, ECS::components::TextureRectComponent{0, 0, (int)texture->getSize().x, 100, 4, 150.0f});
            this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(entity, comp.getX() + 240 * this->_resMult, comp.getY() + 17 * this->_resMult));
        }
    });

}

void Game::handleLaser(Network::Packet &packet)
{
    unsigned int id = packet.getData().getDataUInt();
    unsigned short y = packet.getData().getDataUShort();

    float tmp = 320 * this->_resMult;

    for (int i = 0; i < this->_screenSize.x; i += tmp) {
        --i;

        this->_factory.createLaser(this->_manager.getTexture(Loader::Loader::Laser), i,(y - 80) * this->_resMult, this->_resMult);
    }
    entity_t soundEntity = this->_factory.createSound(client::getAssetPath("songs/effects/laser.ogg"), 1000, true);
    this->_sounds.emplace_back(soundEntity);

}

void Game::handleRay(Network::Packet &packet)
{
    unsigned int id = packet.getData().getDataUInt();
    unsigned short x = packet.getData().getDataUShort();
    unsigned short y = packet.getData().getDataUShort();

    sf::Vector2f rayPos = getRayEntityFromId(id);

    if (rayPos.x == -100) {
        this->_rays.push_back(std::make_pair<size_t, sf::Vector2f>(id, sf::Vector2f(x * this->_resMult, y * this->_resMult)));
    } else {
        for(auto i = _rays.begin(); i != _rays.end(); ++i) {
            if ((*i).first == id) {
                // std::cout << (*i).second.x << " " << (*i).second.y << "|" << x * this->_resMult <<  " " << y * this->_resMult << std::endl;
                this->_fbr->drawLine((*i).second.x, (*i).second.y, x * this->_resMult, y * this->_resMult, sf::Color::Red);
                (*i).second = sf::Vector2f(x * this->_resMult, y * this->_resMult);
                break;
            }
        }
    }
}

void Game::GenParticles()
{
    if (std::chrono::system_clock::now() - this->_lastParticleUpdate > std::chrono::milliseconds(70)) {
        for(auto j = _particles.begin(); j != _particles.end(); ++j) {
            if (std::chrono::system_clock::now() - std::get<2>(*j) > std::chrono::milliseconds(std::get<3>(*j))) {
                j = _particles.erase(j);
                if (j == _particles.end())
                    break;
                continue;
            }
            sf::IntRect area = std::get<0>(*j);
            sf::Color color = std::get<1>(*j);
            for (int i = 0; i < area.width * area.height / 1000; ++i) {
                short x = std::rand() % area.width + area.left;
                short y = std::rand() % area.height + area.top;
                short width = std::rand() % 8 + 3;
                short height = std::rand() % 8 + 3;
                this->_fbr->drawRect(x, y, width, height, color);
            }
        }
        this->_lastParticleUpdate = std::chrono::system_clock::now();
    }
}

void Game::CreateParticle(sf::IntRect rect, sf::Color color, size_t duration)
{
    this->_particles.push_back(std::make_tuple(rect, color, std::chrono::system_clock::now(), duration));
}

void Game::CreateParticle(sf::FloatRect rect, sf::Color color, size_t duration)
{
    this->_particles.push_back(std::make_tuple(sf::IntRect(rect.left, rect.top, rect.width, rect.height), color, std::chrono::system_clock::now(), duration));
}

void Game::handleBonus(Network::Packet &packet)
{
    unsigned char type= packet.getData().getDataUChar();

    auto position = this->ecs.getComponent<ECS::components::SpriteComponent>(this->_playerEntity);
    sf::FloatRect rect = position.getSprite().getGlobalBounds();
    rect.left -= 20 * this->_resMult;
    rect.top -= 20 * this->_resMult;
    rect.width += 40 * this->_resMult;
    rect.height += 40 * this->_resMult;
    if (type == 1)
        this->CreateParticle(rect, sf::Color::Red, 500);
    if (type == 2)
        this->CreateParticle(rect, sf::Color::Green, 500);
    if (type == 3)
        this->CreateParticle(rect, sf::Color::White, 500);
}