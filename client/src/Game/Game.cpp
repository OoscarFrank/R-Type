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
        this->_manager.loadTexture(client::getAssetPath("entity/buttons/matchlist.png"), Loader::toLoad::MatchListButton);

        this->_manager.loadTexture(client::getAssetPath("screens/LooserScreen.png"), Loader::toLoad::LooserScreen);
        this->_manager.loadTexture(client::getAssetPath("screens/menuScreen.png"), Loader::toLoad::MenuScreen);

        this->_manager.loadTexture(client::getAssetPath("entity/missile/missile.png"), Loader::toLoad::Missile);
        this->_manager.loadTexture(client::getAssetPath("entity/missile/missileRed.png"), Loader::toLoad::MissileRed);
        this->_manager.loadTexture(client::getAssetPath("entity/missile/orange_missile.png"), Loader::toLoad::OrangeMissile);
        this->_manager.loadTexture(client::getAssetPath("entity/missile/purple_missile.png"), Loader::toLoad::PurpleMissile);
        this->_manager.loadTexture(client::getAssetPath("entity/missile/green_missile.png"), Loader::toLoad::GreenMissile);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/monster1.png"), Loader::toLoad::Monster1);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/mob1.png"), Loader::toLoad::Monster2);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/mob2.png"), Loader::toLoad::Monster3);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/mob3.png"), Loader::toLoad::Monster4);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/boss1.png"), Loader::toLoad::Boss1);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/boss2.png"), Loader::toLoad::Boss2);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/boss1.png"), Loader::toLoad::Boss3);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/boss1.png"), Loader::toLoad::Boss4);
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

        this->_manager.loadTexture(client::getAssetPath("entity/player/PlayerLifeOutline.png"), Loader::toLoad::playerLifeOutline);
        this->_manager.loadTexture(client::getAssetPath("entity/player/playerLifeContent.png"), Loader::toLoad::playerLifeContent);

        this->_manager.loadFont(client::getAssetPath("fonts/arial.ttf"), Loader::toLoad::Arial);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(84);
    }
    if (mode.isValid()) {
        this->_window.create(mode, "R-TYPE", sf::Style::Fullscreen);
    } else {
        this->_window.create(sf::VideoMode(mode.width, mode.height), "R-TYPE");
    }

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

    this->_window.setFramerateLimit(120);
    this->_lastTime = NOW;
    this->eventMemory = 0;
    this->_gameState = gameState::MENU;

    this->_musics.emplace(EntityManager::MUSIC_TYPE::SOUND_OF_SPACE, this->_factory.createMusic(client::getAssetPath("songs/SOUND_OF_SPACE.ogg"), 100, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::TURN_ON_THE_LIGHTS, this->_factory.createMusic(client::getAssetPath("songs/TURN_ON_THE_LIGHTS.ogg"), 100, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::PUSH_UP, this->_factory.createMusic(client::getAssetPath("songs/PUSH_UP.ogg"), 100, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::VOIS_SUR_TON_CHEMIN, this->_factory.createMusic(client::getAssetPath("songs/VOIS_SUR_TON_CHEMIN.ogg"), 100, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::HEUTE_NACHT, this->_factory.createMusic(client::getAssetPath("songs/HEUTE_NACHT.ogg"), 100, true));
    this->_musics.emplace(EntityManager::MUSIC_TYPE::CLEON, this->_factory.createMusic(client::getAssetPath("songs/CLEON.ogg"), 100, true));

    this->_resMult = static_cast<float>(this->_screenSize.x)/ SCREEN_WIDTH;

    this->_parallax.push_back(this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxFirstbkg), (-0.070f * _resMult), sf::Vector2f(_resMult, _resMult), _resMult));
    this->_parallax.push_back(this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxSecondbkg), (-0.1f * _resMult), sf::Vector2f(_resMult, _resMult), _resMult));

    this->initButtons();
    this->initMenus();
    this->_menuManager.enableMenu(MenuManager::MENU_TYPE::MAIN_MENU);

    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::RedPixel), _screenSize.x, _screenSize.y));
    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::GreenPixel), _screenSize.x, _screenSize.y));
    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::BluePixel), _screenSize.x, _screenSize.y));
    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::YellowPixel), _screenSize.x, _screenSize.y));
    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::PurplePixel), _screenSize.x, _screenSize.y));
    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::CyanPixel), _screenSize.x, _screenSize.y));
    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::WhitePixel), _screenSize.x, _screenSize.y));

    entity_t soundEntity = this->_factory.createSound(client::getAssetPath("songs/piou.ogg"), 1000);
    this->_sounds.emplace(EntityManager::SOUND_TYPE::TEST, soundEntity);

    _pingText = this->_factory.createText("Ping: 0", this->_manager.getFont(Loader::Loader::Arial), this->_screenSize.x - 300, 10, 15);

    this->_screens.emplace(EntityManager::SCREEN_TYPE::MAIN_MENU, this->_factory.createScreen(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::MenuScreen)));
}

Game::~Game()
{
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
        [&](void) {
            std::cout << "feur: " << std::endl;
        });
        this->_menuManager.createButton((MenuManager::BUTTON_TYPE)(MenuManager::BUTTON_TYPE::ROOM_LIST_0 + buttonNbr), newButton);
    }

    this->_menuManager.createButton(MenuManager::BUTTON_TYPE::LEAVE_GAME, this->_factory.createButton((this->_screenSize.x / 2) - (tmpSizebutton / 2), 800.0f + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::QuitButton), sf::Vector2f(_resMult, _resMult),
    [&](void) {
        Stream out;
        out << 24_uc;
        this->_net.send(out);

        this->_gameState = gameState::MENU;
        this->_menuManager.disableAllmenu();
        this->_menuManager.enableMenu(MenuManager::MENU_TYPE::MAIN_MENU);

        for (auto &e : this->_missiles) {
            this->ecs.kill_entity(e.second);
        }
        this->_missiles.clear();
        for (auto &e : this->_ennemies) {
            this->ecs.kill_entity(e.second);
        }
        this->_ennemies.clear();
        for (auto &e : this->_players) {
            this->ecs.kill_entity(e.second);
        }
        this->_players.clear();

        // this->ecs.kill_entity(this->_playerEntity);
        // this->_playerEntity = 0;

        this->ecs.kill_entity(this->_scoreCoche);
        this->_scoreCoche = 0;

        for (auto &e : _loadingBar) {
            this->ecs.kill_entity(e.second);
        }
        this->_loadingBar.clear();

        // this->ecs.kill_entity(this->_timerText);
        // this->_timerText = 0;

        this->ecs.kill_entity(this->_scoreText);
        this->_scoreText = 0;

        this->ecs.kill_entity(this->_gameTimeText);
        this->_gameTimeText = 0;

        this->ecs.kill_entity(this->_looser);
        this->_looser = 0;

        for (auto &e : this->_strobes) {
            this->ecs.disableEntity(e);
        }
    }
    ));
}

void Game::initMenus()
{
    entity_t entity_mainMenu = this->ecs.spawn_entity();
    this->ecs.emplace_component<ECS::components::ControllableComponent>(entity_mainMenu, ECS::components::ControllableComponent{sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Enter});
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
    this->ecs.emplace_component<ECS::components::ControllableComponent>(entity_looseMenu, ECS::components::ControllableComponent{ sf::Keyboard::Key::Enter, sf::Keyboard::Key::Right});
    this->_menuManager.createMenu(MenuManager::MENU_TYPE::LOOSER_MENU, entity_looseMenu, false, std::vector<MenuManager::BUTTON_TYPE>({MenuManager::BUTTON_TYPE::LEAVE_GAME}));
}

void Game::update()
{
    Network::Packet packet;

    while (this->_net.getQueueIn().tryPop(packet)) {
        switch (packet.getInstruction()) {
            case 10:
                this->handleRoomJoin(packet);
                break;
            case 11:
                this->handleTimeoutMatchmaking(packet);
                break;
            case 13:
                this->handlePlayerJoinGame(packet);
                break;
            case 14:
                this->handlePlayerDisconnected(packet);
                break;
            case 17:
                this->handleGameEnd(packet);
                break;
            case 23:
                this->handleLatency(packet);
                break;
            case 27:
                this->handleListRooms(packet);
            default:
                break;
        }
        if (this->_gameState == gameState::MENU)
            continue;
        switch (packet.getInstruction()) {
            case 3:
                this->handlePlayerPosition(packet);
                break;
            case 4:
                this->handleMissilePosition(packet);
                break;
            case 6:
                this->handlePlayerScore(packet);
                break;
            case 7:
                this->handleEnnemiPosition(packet);
                break;
            case 15:
                this->handleMissileDeath(packet);
                break;
            case 16:
                this->handleEnnemiDeath(packet);
                break;
            case 18:
                this->handlePlayerDeath(packet);
                break;
            case 19:
                this->handlePlayerLife(packet);
                break;
            case 20:
                this->handleMonsterLife(packet);
                break;
            case 21:
                this->handleStrobes(packet);
                break;
            case 22:
                this->handleChangeLevel(packet);
                break;
            case 28:
                this->handleBonusPosition(packet);
                break;
            case 29:
                this->handleBonusDestroyed(packet);
                break;
            case 255:
                this->handleResend(packet);
                break;
            default:
                break;
        }
    }
    if (std::chrono::system_clock::now() - this->_lastPing > std::chrono::seconds(1)) {
        this->_lastPing = std::chrono::system_clock::now();
        Stream out;
        if (this->_gameState == gameState::MENU)
            out << 26_uc;
        else
            out << 12_uc << static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(_lastPing.time_since_epoch()).count());
        this->_net.send(out);
    }

    if (this->_gameState == gameState::GAME) {
        float timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->_startGameTime).count() / 1000.0f;

        int minutes = static_cast<int>(timer) / 60;
        int seconds = static_cast<int>(timer) % 60;

        std::ostringstream ss;
        ss << "Timer: " << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;

        if (_gameTimeText == 0) {
            _gameTimeText = this->_factory.createText(ss.str(), this->_manager.getFont(Loader::Loader::Arial), this->_screenSize.x / 2, this->topLeftOffeset.y + 10, 20);
        } else {
            this->_texts.insert(std::make_pair(_gameTimeText, ss.str()));
        }
    }
}

void Game::sendMoveToServer()
{
    for (auto i = this->_entityEvents.begin(); i != this->_entityEvents.end(); ++i) {
        if ((this->_gameState == gameState::MENU || this->_gameState == gameState::ENDGAME)
            && (*i).getEntity() == this->_menuManager.getMenuEntity(MenuManager::MENU_TYPE::MAIN_MENU)) {
            if (((*i).getEvent() & ENTER)) {
                this->_menuManager.executeButtonInMenu(this->ecs);
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
            if ((*i).getEvent() & SPACE && std::chrono::system_clock::now() - this->_lastPlayerFireTime > std::chrono::milliseconds(160)) {
                this->_lastPlayerFireTime = std::chrono::system_clock::now();
                Stream out;
                out << 5_uc;
                this->_net.send(out);
                auto soundComponent = this->ecs.getComponent<ECS::components::SoundComponent>(this->_sounds[EntityManager::SOUND_TYPE::TEST]);
                soundComponent.playSound();
            }
            continue;
        }
    }
    this->_entityEvents.clear();
}


int Game::MainLoop()
{
    while (this->_window.isOpen()) {
        this->refreshScreenSize();
        long currentTime = NOW;
        float deltaTime = (currentTime - this->_lastTime) / 1.0f;
        this->_lastTime = currentTime;
        this->update();

        // ALL SYSTEMS CALL HERE
        ECS::systems::ControllableSystem().update(this->ecs, this->_entityEvents, this->_window, this->eventMemory);
        ECS::systems::PositionSystem().update(this->ecs, this->topLeftOffeset);
        ECS::systems::AnimationSystem().update(this->ecs, deltaTime);
        ECS::systems::ParallaxSystem().update(this->ecs, deltaTime, this->topLeftOffeset);
        ECS::systems::MovableSystem().update(this->ecs, this->_entityPositions, this->topLeftOffeset);
        ECS::systems::ScaleSystem().update(this->ecs);
        ECS::systems::TextSystem().update(this->ecs, this->_texts);
        this->_window.clear();

        // DRAW SYSTEM CALL HERE
        ECS::systems::DrawSystem().update(this->ecs, this->_window);
        this->_window.display();
        this->sendMoveToServer();
    }
    this->_net.setClosed(true);
    return 0;
}



// UPDATE HANDLE FUNCTIONS

void Game::handlePlayerPosition(Network::Packet &packet)
{
    unsigned int id;
    unsigned short x;
    unsigned short y;
    packet >> id >> x >> y;
    x *= this->_resMult;
    y *= this->_resMult;

    this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(this->getPlayerEntityFromId(id), x, y));
}

void Game::handleMissilePosition(Network::Packet &packet)
{
    unsigned int id;
    unsigned char type;
    unsigned short x;
    unsigned short y;
    packet >> id >> type >> x >> y;
    x *= this->_resMult;
    y *= this->_resMult;

    entity_t res = getMissileEntityFromId(id);

    if (res == 0) {
        switch ((int)type) {
            case 1: {
                entity_t newEntity = this->_factory.createMissile(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Missile));
                this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                this->_missiles.push_back(std::make_pair(id, newEntity));
            }
                break;
            case 2: {
                entity_t newEntity = this->_factory.createMissile(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Missile));
                this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                this->_missiles.push_back(std::make_pair(id, newEntity));
            }
                break;
            case 3: {
                entity_t newEntity = this->_factory.createMissileAnnimated(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::OrangeMissile), this->_resMult);
                this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                this->_missiles.push_back(std::make_pair(id, newEntity));
            }
                break;
            case 4: {
                entity_t newEntity = this->_factory.createMissileAnnimated(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::PurpleMissile), this->_resMult);
                this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                this->_missiles.push_back(std::make_pair(id, newEntity));
            }
                break;
            case 5: {
                entity_t newEntity = this->_factory.createMissileAnnimated(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::GreenMissile), this->_resMult);
                this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                this->_missiles.push_back(std::make_pair(id, newEntity));
            }
                break;
            case 6: {
                entity_t newEntity = this->_factory.createMissile(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::MissileRed));
                this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
                this->_missiles.push_back(std::make_pair(id, newEntity));
            }
                break;
            case 7: {
                entity_t newEntity = this->_factory.createMissile(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::MissileRed));
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
    if (_scoreText == 0) {
        _scoreText = this->_factory.createText(ss.str(), this->_manager.getFont(Loader::Loader::Arial), this->_screenSize.x / 2 - (250 * this->_resMult), 10, 20);
    } else {
        this->_texts.insert(std::make_pair(_scoreText, ss.str()));
    }
}

void Game::handleEnnemiPosition(Network::Packet &packet)
{
    unsigned int id;
    unsigned char type;
    unsigned short x;
    unsigned short y;
    packet >> id >> type >> x >> y;
    x *= this->_resMult;
    y *= this->_resMult;

    entity_t res = getEnnemiEntityFromId(id);
    if (res == 0) {
        switch ((int)type) {
            case 2: {
                entity_t newEntity = this->_factory.createEnnemi4frames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Monster2), this->_resMult);
                this->_ennemies.push_back(std::make_pair(id, newEntity));
            }
                break;
            case 3: {
                entity_t newEntity = this->_factory.createEnnemi4frames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Monster3), this->_resMult);
                this->_ennemies.push_back(std::make_pair(id, newEntity));
            }
                break;
            case 4: {
                entity_t newEntity = this->_factory.createEnnemi4frames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Monster4), this->_resMult);
                this->_ennemies.push_back(std::make_pair(id, newEntity));
            }
                break;
            case 5: {
                entity_t newEntity = this->_factory.createEnnemi4frames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Monster4), this->_resMult);
                this->_ennemies.push_back(std::make_pair(id, newEntity));
            }
            case 6: {
                entity_t newEntity = this->_factory.createEnnemi4frames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Boss1), this->_resMult);
                this->_ennemies.push_back(std::make_pair(id, newEntity));
            }
                break;
            case 7: {
                entity_t newEntity = this->_factory.createEnnemi4frames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Boss2), this->_resMult);
                this->_ennemies.push_back(std::make_pair(id, newEntity));
            }
                break;
            case 8: {
                entity_t newEntity = this->_factory.createEnnemi4frames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Boss3), this->_resMult);
                this->_ennemies.push_back(std::make_pair(id, newEntity));
            }
                break;
            case 9: {
                entity_t newEntity = this->_factory.createEnnemi4frames(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Boss4), this->_resMult);
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
    entity_t newEntity = this->_factory.createPlayer(50.0f, 200.0f + (this->_playerId * 100), texture);
    this->_players.push_back(std::make_pair(this->_playerId, newEntity));
    this->_playerEntity = newEntity;
    this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});

    this->_scoreCoche = this->_factory.createScoreCoche((((this->_window.getSize().x / 2) - ((660.0 * this->_resMult) / 2))), this->topLeftOffeset.y , this->_manager.getTexture(Loader::Loader::ScoreCoche), this->_resMult);

    entity_t playerLifeBar = this->_factory.createLoadingBar(this->topLeftOffeset.x + 10.0f, this->topLeftOffeset.y + this->_screenSize.y - (100.0f * this->_resMult), this->_manager.getTexture(Loader::Loader::playerLifeOutline), this->_manager.getTexture(Loader::Loader::playerLifeContent), this->_resMult);
    this->_loadingBar.insert(std::make_pair(EntityManager::LOADINGBAR_TYPE::PLAYER_LIFE, playerLifeBar));

    auto it = this->_screens.find(SCREEN_TYPE::MAIN_MENU);
    if (it != this->_screens.end()) {
        this->ecs.kill_entity(it->second);
    }
}

void Game::handleTimeoutMatchmaking(Network::Packet &packet)
{
    packet >> this->_startTimeLeft >> this->_started >> this->currentSong;

    if (this->_started == true) {
        this->ecs.kill_entity(_timerText);
        this->_gameState = gameState::GAME;
        this->_startGameTime = std::chrono::system_clock::now();
        _scoreText = this->_factory.createText("Score: 0", this->_manager.getFont(Loader::Loader::Arial), this->_screenSize.x / 2 - (250 * this->_resMult), this->topLeftOffeset.y + 10, 20);
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
        if (_timerText == 0) {
            _timerText = this->_factory.createText(ss.str(), this->_manager.getFont(Loader::Loader::Arial), this->_screenSize.x / 2 - 115, this->topLeftOffeset.y + 10, 30);
        } else {
            this->_texts.insert(std::make_pair(_timerText, ss.str()));
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
        entity_t newEntity = this->_factory.createPlayer(50.0f, 200.0f + (id * 100), texture);
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
    unsigned short x;
    unsigned short y;
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
        this->_looser = this->_factory.createScreen(this->topLeftOffeset.x, this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::LooserScreen));
        this->ecs.emplace_component<ECS::components::ScaleComponent>(this->_looser, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
        this->_menuManager.enableMenu(MenuManager::MENU_TYPE::LOOSER_MENU);
        this->_gameState = gameState::ENDGAME;
    }
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
                int volume = 100 * timeout / fadeOutTime;
                volume = (volume * -1) + 100;
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
    if (_pingText == 0) {
        _pingText = this->_factory.createText(ss.str(), this->_manager.getFont(Loader::Loader::Arial), this->_screenSize.x - 100, 10, 15);
    } else {
        this->_texts.insert(std::make_pair(_pingText, ss.str()));
    }
}

void Game::handleListRooms(Network::Packet &packet)
{
    u_int roomId;
    u_char nbrPlayers;
    u_char maxPlayers;
    bool joinable;
    packet >> roomId >> nbrPlayers >> maxPlayers >> joinable;

    // chercher si la room existe deja dans _roomsData
    // si non:
    // -> create button
    // -> create text for room id
    // -> create text for nbr players / max players
    // -> stock roomId on key, button , text and text in a tuple
    // -> register button in menu manager
    // -> add button to menu button list in menu manager
    // si oui:
    // -> update text for room id
    // -> update text for nbr players / max players

    // if (this->_roomsData.find(roomId) != this->_roomsData.end()) {
    //     if (_roomsData.size() >= 6) {
    //         return;
    //     }
    //     this->ecs.modify_component<ECS::components::TextComponent>(this->_roomsData[roomId].first, [roomId](ECS::components::TextComponent &comp) {
    //         comp.setString(std::to_string(roomId));
    //     });
    //     this->ecs.modify_component<ECS::components::TextComponent>(this->_roomsData[roomId].second, [nbrPlayers, maxPlayers](ECS::components::TextComponent &comp) {
    //         comp.setString(std::to_string(nbrPlayers) + "/" + std::to_string(maxPlayers));
    //     });
    //     return;

    //     entity_t text1 = this->_factory.createText(std::to_string(roomId), this->_manager.getFont(Loader::Loader::Arial), this->_screenSize.x / 2 - (200 * this->_resMult), this->topLeftOffeset.y + 100 + (this->_roomsData.size() * 100), 20);
    //     entity_t text2 = this->_factory.createText(std::to_string(nbrPlayers) + "/" + std::to_string(maxPlayers), this->_manager.getFont(Loader::Loader::Arial), this->_screenSize.x / 2 - (200 * this->_resMult), this->topLeftOffeset.y + 100 + (this->_roomsData.size() * 100) + 30, 20);
    //     _roomsData[roomId] = std::make_pair(text1, text2);
    // }


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
        this->ecs.kill_entity(entity);

        this->_entityPositions.erase(std::remove_if(this->_entityPositions.begin(), this->_entityPositions.end(), [id](ECS::systems::MovableSystem::EntityPos const &pair) {
            return pair.getEntity() == id;
        }), this->_entityPositions.end());

        this->_bonuses.erase(std::remove_if(this->_bonuses.begin(), this->_bonuses.end(), [id](std::pair<unsigned int, entity_t> const &pair) {
            return pair.first == id;
        }), this->_bonuses.end());
    }
}
