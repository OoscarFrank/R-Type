#include "./Game.hpp"
#include "../ECS/Components/Components.hpp"
#include "../ECS/Systems/Systems.hpp"

using namespace game;
using entity_t = std::size_t;

Game::Game(std::string ip, int port) :
    _manager(Loader()),
    _factory(Factory(ecs)),
    _net(Network(ip, port)),
    _roomId(0),
    _playerId(0),
    _startTimeLeft(0),
    _started(0),
    _lastPing(std::chrono::system_clock::now())
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    this->_manager.loadTexture(client::getAssetPath("entity/BlackPixel.png"), Loader::toLoad::BlackPixel);
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
    this->_gameOver = false;
    this->_menuEntity = -1;
    this->_manager.createMusic(client::getAssetPath("songs/song.ogg"));

    this->_resMult = static_cast<float>(this->_screenSize.x)/ SCREEN_WIDTH;

    this->_parallax.push_back(this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxFirstbkg), (-0.070f * _resMult), _resMult));
    this->_parallax.push_back(this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxSecondbkg), (-0.1f * _resMult), _resMult));
    this->ecs.emplace_component<ECS::components::ScaleComponent>(this->_parallax[0], ECS::components::ScaleComponent{_resMult, _resMult});
    this->ecs.emplace_component<ECS::components::ScaleComponent>(this->_parallax[1], ECS::components::ScaleComponent{_resMult, _resMult});
    this->_menuEntity = this->ecs.spawn_entity();
    this->ecs.emplace_component<ECS::components::ControllableComponent>(this->_menuEntity, ECS::components::ControllableComponent{sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Enter});

    this->_buttons.push_back(this->_factory.createButton(100.0f + this->topLeftOffeset.x, 100.0f + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::CreateRoomButton)));
    this->_buttons.push_back(this->_factory.createButton(100.0f + this->topLeftOffeset.x, 200.0f + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::JoinRoomButton)));
    this->_buttons.push_back(this->_factory.createButton(100.0f + this->topLeftOffeset.x, 300.0f + this->topLeftOffeset.y, this->_manager.getTexture(Loader::Loader::QuitButton)));
    this->ecs.emplace_component<ECS::components::ScaleComponent>(this->_buttons[0], ECS::components::ScaleComponent{_resMult, _resMult});
    this->ecs.emplace_component<ECS::components::ScaleComponent>(this->_buttons[1], ECS::components::ScaleComponent{_resMult, _resMult});
    this->ecs.emplace_component<ECS::components::ScaleComponent>(this->_buttons[2], ECS::components::ScaleComponent{_resMult, _resMult});
}

Game::~Game()
{
    this->_manager.stopMusic();
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
            default:
                break;
        }
    }
    if (std::chrono::system_clock::now() - this->_lastPing > std::chrono::seconds(2)) {
        this->_lastPing = std::chrono::system_clock::now();
        this->_net.setInst(12);
        this->_net.send();
    }
}

void Game::sendMoveToServer()
{
    for (auto i = this->_entityEvents.begin(); i != this->_entityEvents.end(); ++i) {
        if (!this->_gameOver && (*i).getEntity() == this->_playerEntity) {
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
    x *= this->_resMult;

    unsigned short y = packet.getData().getDataUShort();
    y *= this->_resMult;

    this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(this->getPlayerEntityFromId(id), x, y));
}

void Game::handleMissilePosition(Network::Packet &packet)
{
    unsigned int id = packet.getData().getDataUInt();
    unsigned char type = packet.getData().getDataUChar();

    unsigned short x = packet.getData().getDataUShort();
    x *= this->_resMult;

    unsigned short y = packet.getData().getDataUShort();
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
    unsigned int score = packet.getData().getDataUInt();

    std::cout << "Score: " << score << std::endl;
}

void Game::handleEnnemiPosition(Network::Packet &packet)
{
    unsigned int id = packet.getData().getDataUInt();

    unsigned char type = packet.getData().getDataUChar();

    unsigned short x = packet.getData().getDataUShort();
    x *= this->_resMult;

    unsigned short y = packet.getData().getDataUShort();
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
            default:
                break;
        }
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

    entity_t newEntity = this->_factory.createPlayer(-1000.0f, -1000.0f, texture);
    this->_players.push_back(std::make_pair(this->_playerId, newEntity));
    this->_playerEntity = newEntity;
    this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});

    this->_scoreCoche = this->_factory.createScoreCoche((((this->_window.getSize().x / 2) - ((660.0 * this->_resMult) / 2))), 0.0f, this->_manager.getTexture(Loader::Loader::ScoreCoche), this->_resMult);

    // ECS::systems::ScaleSystem().update(this->ecs);
}

void Game::handleTimeoutMatchmaking(Network::Packet &packet)
{
    this->_startTimeLeft = packet.getData().getDataUInt();
    this->_started = packet.getData().getDataUChar();
    this->currentSong = packet.getData().getDataUChar();
    
    if (this->_started == true) {
        std::cout << (int)this->currentSong << std::endl;
        for (auto &parallax : this->_parallax ) {
            this->ecs.modify_component<ECS::components::ParallaxComponent>(parallax, [](ECS::components::ParallaxComponent &comp) {
                comp.setScrollSpeed(comp.getScrollSpeed() * 4.0f);
            });
        }
        this->_playerLife = this->_factory.createPlayerLife(10.0f, this->_window.getSize().y - (100.0f * this->_resMult), this->_manager.getTexture(Loader::Loader::PlayerLife), this->_resMult);
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
        entity_t newEntity = this->_factory.createPlayer(-1000.0f, -1000.0f, texture);
        this->_players.push_back(std::make_pair(id, newEntity));
        this->ecs.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{this->_resMult, this->_resMult});

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
    x *= this->_resMult;

    unsigned short y = packet.getData().getDataUShort();
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
    int life = packet.getData().getDataUInt();
    if (this->_playerLife == 0)
        return;
    this->ecs.modify_component<ECS::components::TextureRectComponent>(this->_playerLife, [life](ECS::components::TextureRectComponent &comp) {
        comp.setFrameOnTexture(life / 10);
    });
}
