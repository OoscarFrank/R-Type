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
    this->_net.setInst(9);
    this->_net.send();

    this->_manager.loadTexture("assets/parallax/background.png", Loader::toLoad::ParallaxFirstbkg);
    this->_manager.loadTexture("assets/parallax/background2.png", Loader::toLoad::ParallaxSecondbkg);

    int  divider = 1;
    #ifdef SFML_SYSTEM_MACOS
        divider = 2;
    #endif
    _resMult = (float)(this->_screenSize.x / divider)/ SCREEN_WIDTH;

    this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxFirstbkg), -0.15f);
    this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxSecondbkg), -0.20f);
}

Game::~Game()
{
}

entity_t Game::getPlayerEntityFromId(unsigned char id)
{
    for (auto &player : this->_players) {
        if (player.first == id)
            return player.second;
    }
    return 0;
}

entity_t Game::getMissileEntityFromId(unsigned char id)
{
    for (auto &missile : this->_missiles) {
        if (missile.first == id)
            return missile.second;
    }
    return 0;
}

entity_t Game::getEnnemieEntityFromId(unsigned char id)
{
    for (auto &ennemie : this->_ennemies) {
        if (ennemie.first == id)
            return ennemie.second;
    }
    return 0;
}

void Game::update()
{
    Network::Packet packet;

    while (this->_net.getQueueIn().tryPop(packet)) {
        if (packet.getInstruction() == 10) {
        this->_roomId = packet.getData().getDataUShort();
            this->_playerId = packet.getData().getDataUChar();
            this->_manager.loadTexture("assets/player/move.png", Loader::toLoad::Player);

            entity_t newEntity = this->_factory.createPlayer(-1000.0f, -1000.0f, this->_manager.getTexture(Loader::Loader::Player));
            this->_players.push_back(std::make_pair(this->_playerId, newEntity));
            this->_playerEntity = newEntity;

            this->_manager.loadTexture("assets/entity/rocket.png", Loader::toLoad::Rocket);
            this->_manager.loadTexture("assets/monsters/monster1.png", Loader::toLoad::Monster1);
        }

        if (packet.getInstruction() == 11) {
            this->_startTimeLeft = packet.getData().getDataUInt();
            this->_started = packet.getData().getDataUChar();
        }

        if (packet.getInstruction() == 3) {
            unsigned char id = packet.getData().getDataUChar();
            unsigned short x = packet.getData().getDataUShort();
            x *= _resMult;
            unsigned short y = packet.getData().getDataUShort();
            y *= _resMult;
            this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(this->getPlayerEntityFromId(id), x, y));
        }

        if (packet.getInstruction() == 4) {
            unsigned int id = packet.getData().getDataUInt();
            unsigned char type = packet.getData().getDataUChar();
            unsigned short x = packet.getData().getDataUShort();
            x *= _resMult;
            unsigned short y = packet.getData().getDataUShort();
            y *= _resMult;

            entity_t res = getMissileEntityFromId(id);

            if (res == 0) {
                entity_t newEntity = this->_factory.createMissile(x, y, this->_manager.getTexture(Loader::Loader::Rocket));
                this->_missiles.push_back(std::make_pair(id, newEntity));
            } else {
                this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(res, x, y));
            }
        }

        if (packet.getInstruction() == 7) {
            unsigned char id = packet.getData().getDataUChar();
            unsigned short x = packet.getData().getDataUShort();
            x *= _resMult;
            unsigned short y = packet.getData().getDataUShort();
            y *= _resMult;

            entity_t res = getEnnemieEntityFromId(id);

            if (res == 0) {
                entity_t newEntity = this->_factory.createEnnemie(x, y, this->_manager.getTexture(Loader::Loader::Player));
                this->_ennemies.push_back(std::make_pair(id, newEntity));
            } else {
                this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(res, x, y));
            }
        }
    }
    this->_net.setInst(12);
    this->_net.send();
}

void Game::sendMoveToServer()
{
    for (auto i = this->_entityMoves.begin(); i != this->_entityMoves.end(); ++i) {
        if ((*i).getEntity() == this->_playerEntity) {
            this->_net.setInst(2);
            this->_net.getStreamOut().setDataUChar((*i).getMove());
            this->_net.getStreamOut().setDataUChar(1);
            this->_net.send();
        }
    }
    this->_entityMoves.clear();
}

int Game::MainLoop()
{
    while (this->_window.isOpen()) {
        long currentTime = NOW;
        float deltaTime = (currentTime - _lastTime) / 1.0f;
        _lastTime = currentTime;
        this->update();
        this->EventLoop(this->_window, this->_net);
        // ALL SYSTEMS CALL HERE (update)
        ECS::systems::ControllableSystem().update(this->ecs, this->_entityMoves);
        ECS::systems::PositionSystem().update(this->ecs);
        ECS::systems::AnimationSystem().update(this->ecs, deltaTime);
        ECS::systems::ParallaxSystem().update(this->ecs, deltaTime);
        ECS::systems::MovableSystem().update(this->ecs, this->_entityPositions);
        this->_window.clear();
        // DRAW SYSTEM CALL HERE (update) (after clear) (before display) (no update) (no event) (no loop) (no system call) (no event loop)
        ECS::systems::DrawSystem().update(this->ecs, this->_window);
        this->_window.display();
        this->sendMoveToServer();
    }
    return 0;
}
