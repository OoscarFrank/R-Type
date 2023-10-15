#include "./Game.hpp"
#include "./ECS/Components/Components.hpp"
#include "./ECS/Systems/Systems.hpp"

using namespace game;
using entity_t = std::size_t;

Game::Game() :
    _manager(Loader()),
    _factory(Factory(ecs)),
    _net(Network()),
    _roomId(0),
    _playerId(0),
    _startTimeLeft(0),
    _started(0)
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    this->_screenSize = {mode.width, mode.height};

    if (mode.isValid()) {
        this->_window.create(mode, "R-TYPE", sf::Style::Fullscreen);
    } else {
        this->_window.create(sf::VideoMode(mode.width, mode.height), "R-TYPE");
    }

    this->_window.setFramerateLimit(120);
    this->_lastTime = NOW;
    this->eventMemory = 0;
    this->_gameOver = false;
    this->_menuEntity = -1;

    this->_manager.loadTexture(client::getAssetPath("parallax/background.png"), Loader::toLoad::ParallaxFirstbkg);
    this->_manager.loadTexture(client::getAssetPath("parallax/background2.png"), Loader::toLoad::ParallaxSecondbkg);

    this->_manager.loadTexture(client::getAssetPath("entity/buttons/CreateRoomButton.png"), Loader::toLoad::CreateRoomButton);
    this->_manager.loadTexture(client::getAssetPath("entity/buttons/JoinRoomButton.png"), Loader::toLoad::JoinRoomButton);
    this->_manager.loadTexture(client::getAssetPath("entity/buttons/QuitButton.png"), Loader::toLoad::QuitButton);
    this->_manager.loadTexture(client::getAssetPath("screens/LooserScreen.png"), Loader::toLoad::LooserScreen);

    int  divider = 1;
    #ifdef SFML_SYSTEM_MACOS
        divider = 2;
    #endif
    this->_resMult = (float)(this->_screenSize.x / divider)/ SCREEN_WIDTH;

    this->_parallax.push_back(this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxFirstbkg), -0.035f));
    this->_parallax.push_back(this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxSecondbkg), -0.05f));

    this->_menuEntity = this->ecs.spawn_entity();
    this->ecs.emplace_component<ECS::components::ControllableComponent>(this->_menuEntity, ECS::components::ControllableComponent{sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Enter});

    this->_buttons.push_back(this->_factory.createButton(100.0f, 100.0f, this->_manager.getTexture(Loader::Loader::CreateRoomButton)));
    this->_buttons.push_back(this->_factory.createButton(100.0f, 200.0f, this->_manager.getTexture(Loader::Loader::JoinRoomButton)));
    this->_buttons.push_back(this->_factory.createButton(100.0f, 300.0f, this->_manager.getTexture(Loader::Loader::QuitButton)));
}

Game::~Game()
{
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
            default:
                break;
        }
    }
    this->_net.setInst(12);
    this->_net.send();
}

void Game::sendMoveToServer()
{
    for (auto i = this->_entityEvents.begin(); i != this->_entityEvents.end(); ++i) {
        if (!_gameOver && (*i).getEntity() == this->_playerEntity) {
            char move = (*i).getEvent() & (UP | DOWN | LEFT | RIGHT);
            if ((*i).getEvent() & move) {
                this->_net.setInst(2);
                this->_net.getStreamOut().setDataUChar((*i).getEvent() & move);
                this->_net.getStreamOut().setDataUChar(1);
                this->_net.send();
            }
            if ((*i).getEvent() & SPACE) {
                this->_net.setInst(5);
                this->_net.send();
            }
        }
        if (this->_menuEntity != -1 && (*i).getEntity() == this->_menuEntity) {
            if ((*i).getEvent() & ENTER) {
                this->ecs.kill_entity(this->_menuEntity);
                this->_menuEntity = -1;
                this->_net.setInst(9);
                this->_net.send();
            }
        }
    }
    this->_entityEvents.clear();
}

int Game::MainLoop()
{
    while (this->_window.isOpen()) {
        long currentTime = NOW;
        float deltaTime = (currentTime - _lastTime) / 1.0f;
        _lastTime = currentTime;
        this->update();
        // ALL SYSTEMS CALL HERE
        ECS::systems::ControllableSystem().update(this->ecs, this->_entityEvents, this->_window, this->eventMemory);
        ECS::systems::PositionSystem().update(this->ecs);
        ECS::systems::AnimationSystem().update(this->ecs, deltaTime);
        ECS::systems::ParallaxSystem().update(this->ecs, deltaTime);
        ECS::systems::MovableSystem().update(this->ecs, this->_entityPositions);
        ECS::systems::ScaleSystem().update(this->ecs);
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
    unsigned int id = packet.getData().getDataUInt();
    unsigned short x = packet.getData().getDataUShort();
    x *= _resMult;
    unsigned short y = packet.getData().getDataUShort();
    y *= _resMult;
    this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(this->getPlayerEntityFromId(id), x, y));
}

void Game::handleMissilePosition(Network::Packet &packet)
{
    unsigned int id = packet.getData().getDataUInt();
    unsigned char type = packet.getData().getDataUChar();
    unsigned short x = packet.getData().getDataUShort();
    x *= _resMult;
    unsigned short y = packet.getData().getDataUShort();
    y *= _resMult;

    (void)type;

    entity_t res = getMissileEntityFromId(id);
    if (res == 0) {
        entity_t newEntity = this->_factory.createMissile(x, y, this->_manager.getTexture(Loader::Loader::Missile));
        this->_missiles.push_back(std::make_pair(id, newEntity));
    } else {
        this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(res, x, y));
    }
}

void Game::handlePlayerScore(Network::Packet &packet)
{
    unsigned int score = packet.getData().getDataUInt();
    std::cout << "Score: " << score << std::endl;
}

void Game::handleEnnemiPosition(Network::Packet &packet)
{
    unsigned int id = packet.getData().getDataUInt();
    unsigned short x = packet.getData().getDataUShort();
    x *= _resMult;
    unsigned short y = packet.getData().getDataUShort();
    y *= _resMult;

    entity_t res = getEnnemiEntityFromId(id);

    if (res == 0) {
        entity_t newEntity = this->_factory.createEnnemi(x, y, this->_manager.getTexture(Loader::Loader::Monster1)); // TO REPLACE
        this->_ennemies.push_back(std::make_pair(id, newEntity));
    } else {
        this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(res, x, y));
    }
}

void Game::handleRoomJoin(Network::Packet &packet)
{
    for (auto &button : this->_buttons) {
        this->ecs.kill_entity(button);
    }
    this->_roomId = packet.getData().getDataUInt();
    this->_playerId = packet.getData().getDataUInt();

    this->_manager.loadTexture(client::getAssetPath("entity/missile/missile.png"), Loader::toLoad::Missile);
    this->_manager.loadTexture(client::getAssetPath("entity/monsters/monster1.png"), Loader::toLoad::Monster1);
    this->_manager.loadTexture(client::getAssetPath("entity/player/player_move1.png"), Loader::toLoad::Player_move1);
    this->_manager.loadTexture(client::getAssetPath("entity/player/player_move2.png"), Loader::toLoad::Player_move2);
    this->_manager.loadTexture(client::getAssetPath("entity/player/player_move3.png"), Loader::toLoad::Player_move3);
    this->_manager.loadTexture(client::getAssetPath("entity/player/player_move4.png"), Loader::toLoad::Player_move4);

    std::shared_ptr<sf::Texture> texture = nullptr;

    switch (_playerId) {
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

    entity_t newEntity = this->_factory.createPlayer(-1000.0f, -1000.0f, texture);
    this->_players.push_back(std::make_pair(this->_playerId, newEntity));
    this->_playerEntity = newEntity;
}

void Game::handleTimeoutMatchmaking(Network::Packet &packet)
{
    this->_startTimeLeft = packet.getData().getDataUInt();
    this->_started = packet.getData().getDataUChar();
    if (this->_started == true) {
        for (auto &parallax : this->_parallax ) {
            this->ecs.modify_component<ECS::components::ParallaxComponent>(parallax, [](ECS::components::ParallaxComponent &comp) {
                comp.setScrollSpeed(comp.getScrollSpeed() * 4.0f);
            });
        }
    }
}

void Game::handlePlayerJoinGame(Network::Packet &packet)
{
    unsigned int id = packet.getData().getDataUInt();
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
        entity_t newEntity = this->_factory.createPlayer(500.0f, 500.0f, texture);
        this->_players.push_back(std::make_pair(id, newEntity));
    }
}

void Game::handlePlayerDisconnected(Network::Packet &packet)
{
    unsigned int id = packet.getData().getDataUInt();
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
    unsigned int id = packet.getData().getDataUInt();
    unsigned char type = packet.getData().getDataUChar();
    unsigned short x = packet.getData().getDataUShort();
    x *= _resMult;
    unsigned short y = packet.getData().getDataUShort();
    y *= _resMult;

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
    unsigned int id = packet.getData().getDataUInt();
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
    unsigned char type = packet.getData().getDataUChar();
    this->_gameOver = true;
}

void Game::handlePlayerDeath(Network::Packet &packet)
{
    unsigned int id = packet.getData().getDataUInt();
    entity_t res = getPlayerEntityFromId(id);

    this->_looser.push_back(this->_factory.createLooserScreen(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::LooserScreen)));
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
