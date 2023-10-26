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
    _lastPing(std::chrono::system_clock::now())
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    try {
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/BlackPixel.png"), Loader::toLoad::BlackPixel);
        this->_manager.loadTexture(client::getAssetPath("parallax/background.png"), Loader::toLoad::ParallaxFirstbkg);
        this->_manager.loadTexture(client::getAssetPath("parallax/background2.png"), Loader::toLoad::ParallaxSecondbkg);
        this->_manager.loadTexture(client::getAssetPath("entity/buttons/CreateRoomButton.png"), Loader::toLoad::CreateRoomButton);
        this->_manager.loadTexture(client::getAssetPath("entity/buttons/JoinRoomButton.png"), Loader::toLoad::JoinRoomButton);
        this->_manager.loadTexture(client::getAssetPath("entity/buttons/QuitButton.png"), Loader::toLoad::QuitButton);
        this->_manager.loadTexture(client::getAssetPath("screens/LooserScreen.png"), Loader::toLoad::LooserScreen);
        this->_manager.loadTexture(client::getAssetPath("entity/missile/missile.png"), Loader::toLoad::Missile);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/monster1.png"), Loader::toLoad::Monster1);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/mob1.png"), Loader::toLoad::Monster2);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/mob2.png"), Loader::toLoad::Monster3);
        this->_manager.loadTexture(client::getAssetPath("entity/monsters/mob3.png"), Loader::toLoad::Monster4);
        this->_manager.loadTexture(client::getAssetPath("entity/player/player_move1.png"), Loader::toLoad::Player_move1);
        this->_manager.loadTexture(client::getAssetPath("entity/player/player_move2.png"), Loader::toLoad::Player_move2);
        this->_manager.loadTexture(client::getAssetPath("entity/player/player_move3.png"), Loader::toLoad::Player_move3);
        this->_manager.loadTexture(client::getAssetPath("entity/player/player_move4.png"), Loader::toLoad::Player_move4);
        this->_manager.loadTexture(client::getAssetPath("entity/player/PlayerLife.png"), Loader::toLoad::PlayerLife);
        this->_manager.loadTexture(client::getAssetPath("entity/buttons/ScoreCoche.png"), Loader::toLoad::ScoreCoche);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/RedPixel.png"), Loader::toLoad::RedPixel);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/GreenPixel.png"), Loader::toLoad::GreenPixel);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/BluePixel.png"), Loader::toLoad::BluePixel);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/CyanPixel.png"), Loader::toLoad::CyanPixel);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/PurplePixel.png"), Loader::toLoad::PurplePixel);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/YellowPixel.png"), Loader::toLoad::YellowPixel);
        this->_manager.loadTexture(client::getAssetPath("entity/Pixels/WhitePixel.png"), Loader::toLoad::WhitePixel);
        this->_manager.loadFont(client::getAssetPath("fonts/arial.ttf"), Loader::toLoad::Arial);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(84);
    }
    this->_playerLife = 0;

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



    this->_resMult = static_cast<float>(this->_screenSize.x)/ SCREEN_WIDTH;

    this->_parallax.push_back(this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxFirstbkg), (-0.070f * _resMult), sf::Vector2f(_resMult, _resMult), _resMult));
    this->_parallax.push_back(this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxSecondbkg), (-0.1f * _resMult), sf::Vector2f(_resMult, _resMult), _resMult));

    // create buttons
    this->_menuManager.createButton(MenuManager::BUTTON_TYPE::CREATE_GAME, this->_factory.createButton(100.0f + this->topLeftOffeset.x, 100.0f + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::CreateRoomButton), sf::Vector2f(_resMult, _resMult),
    [&](void) {
        this->_menuManager.disableMenu(MenuManager::MENU_TYPE::MAIN_MENU);
        this->_gameState = gameState::MATCHMAKING;
        Stream out;
        out << 8_uc << 0_uc;
        this->_net.send(out);
    }));

    this->_menuManager.createButton(MenuManager::BUTTON_TYPE::JOIN_GAME, this->_factory.createButton(100.0f + this->topLeftOffeset.x, 200.0f + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::JoinRoomButton), sf::Vector2f(_resMult, _resMult),
    [&](void) {
        this->_menuManager.disableMenu(MenuManager::MENU_TYPE::MAIN_MENU);
        this->_gameState = gameState::MATCHMAKING;
        Stream out;
        out << 9_uc;
        this->_net.send(out);
    }
    ));

    this->_menuManager.createButton(MenuManager::BUTTON_TYPE::EXIT_SYSTEM, this->_factory.createButton(100.0f + this->topLeftOffeset.x, 300.0f + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::QuitButton), sf::Vector2f(_resMult, _resMult),
    [&](void) {
        this->_window.close();
    }
    ));

    // create menus
    entity_t entity_mainMenu = this->ecs.spawn_entity();
    this->ecs.emplace_component<ECS::components::ControllableComponent>(entity_mainMenu, ECS::components::ControllableComponent{sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Enter});
    this->_menuManager.createMenu(MenuManager::MENU_TYPE::MAIN_MENU, entity_mainMenu, true, std::vector<MenuManager::BUTTON_TYPE>({MenuManager::BUTTON_TYPE::CREATE_GAME, MenuManager::BUTTON_TYPE::JOIN_GAME, MenuManager::BUTTON_TYPE::EXIT_SYSTEM}));
    this->_menuManager.initFirstButton(MenuManager::BUTTON_TYPE::CREATE_GAME);
    this->_menuManager.enableMenu(MenuManager::MENU_TYPE::MAIN_MENU);

    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::RedPixel), _screenSize.x, _screenSize.y));
    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::GreenPixel), _screenSize.x, _screenSize.y));
    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::BluePixel), _screenSize.x, _screenSize.y));
    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::YellowPixel), _screenSize.x, _screenSize.y));
    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::PurplePixel), _screenSize.x, _screenSize.y));
    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::CyanPixel), _screenSize.x, _screenSize.y));
    _strobes.push_back(this->_factory.createStrobe(this->_manager.getTexture(Loader::Loader::WhitePixel), _screenSize.x, _screenSize.y));

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

void Game::update()
{
    Network::Packet packet;

    while (this->_net.getQueueIn().tryPop(packet)) {
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
            case 15:
                this->handleMissileDeath(packet);
                break;
            case 16:
                this->handleEnnemiDeath(packet);
                break;
            case 17:
                this->handleGameEnd(packet);
                break;
            case 18:
                this->handlePlayerDeath(packet);
                break;
            case 19:
                this->handlePlayerLife(packet);
                break;
            case 21:
                this->handleStrobes(packet);
                break;
            case 22:
                this->handleChangeLevel(packet);
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
        out << 12_uc;
        this->_net.send(out);
    }
}

void Game::sendMoveToServer()
{
    for (auto i = this->_entityEvents.begin(); i != this->_entityEvents.end(); ++i) {
        if (this->_gameState == gameState::MENU && (*i).getEntity() == this->_menuManager.getMenuEntity(MenuManager::MENU_TYPE::MAIN_MENU)) {
            if (((*i).getEvent() & RIGHT) || ((*i).getEvent() & ENTER)) {
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
            if ((*i).getEvent() & SPACE) {
                Stream out;
                out << 5_uc;
                this->_net.send(out);
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

    (void)type;
    entity_t res = getMissileEntityFromId(id);

    if (res == 0) {
        entity_t newEntity = this->_factory.createMissile(x + this->topLeftOffeset.x, y + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::Missile));
        this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});
        this->_missiles.push_back(std::make_pair(id, newEntity));
    } else {
        this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(res, x, y));
    }
}

void Game::handlePlayerScore(Network::Packet &packet)
{
    unsigned int score;
    packet >> score;

    std::cout << "Score: " << score << std::endl;
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

    this->_scoreCoche = this->_factory.createScoreCoche((((this->_window.getSize().x / 2) - ((660.0 * this->_resMult) / 2))), 0.0f, this->_manager.getTexture(Loader::Loader::ScoreCoche), this->_resMult);
}

void Game::handleTimeoutMatchmaking(Network::Packet &packet)
{
    packet >> this->_startTimeLeft >> this->_started >> this->currentSong;

    if (this->_started == true) {
        this->ecs.kill_entity(_timerText);
        this->_gameState = gameState::GAME;
        this->handleMusic(this->ecs, static_cast<EntityManager::MUSIC_TYPE>(this->currentSong), [](ECS::components::MusicComponent &music) {
            music.playMusic();
        });
        for (auto &parallax : this->_parallax ) {
            this->ecs.modify_component<ECS::components::ParallaxComponent>(parallax, [](ECS::components::ParallaxComponent &comp) {
                comp.setScrollSpeed(comp.getScrollSpeed() * 4.0f);
            });
        }
        this->_playerLife = this->_factory.createPlayerLife(10.0f, this->_window.getSize().y - (100.0f * this->_resMult), this->_manager.getTexture(Loader::Loader::PlayerLife), this->_resMult);
    } else {
        float timer = this->_startTimeLeft / 1000.0f;
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(1) << timer;
        if (_timerText == 0) {
            _timerText = this->_factory.createText(ss.str(), this->_manager.getFont(Loader::Loader::Arial), this->_screenSize.x / 2 - 25, 10, 40);
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
        this->_looser.push_back(this->_factory.createLooserScreen(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::LooserScreen)));
        this->ecs.emplace_component<ECS::components::ScaleComponent>(this->_looser[0], ECS::components::ScaleComponent{this->_resMult, this->_resMult});
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
    if (this->_playerLife == 0)
        return;
    this->ecs.modify_component<ECS::components::TextureRectComponent>(this->_playerLife, [life](ECS::components::TextureRectComponent &comp) {
        comp.setFrameOnTexture(life / 10);
    });
}

void Game::handleStrobes(Network::Packet &packet)
{
    unsigned char color;
    unsigned char onOff;
    packet >> color >> onOff;
    if (onOff) {
        this->ecs.modify_component<ECS::components::PositionComponent>(this->_strobes[color - 1], [](ECS::components::PositionComponent &comp) {
            comp.setX(0.0f);
            comp.setY(0.0f);
        });
    } else {
        float x = this->_screenSize.x;
        float y = this->_screenSize.y;
        this->ecs.modify_component<ECS::components::PositionComponent>(this->_strobes[color - 1], [x, y](ECS::components::PositionComponent &comp) {
            comp.setX(x);
            comp.setY(y);
        });
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
    unsigned int fadeOutTime = 5000;


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
