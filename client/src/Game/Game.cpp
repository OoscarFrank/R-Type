#include "./Game.hpp"
#include "./ECS/Components/Components.hpp"
#include "./ECS/Systems/Systems.hpp"
#include "./ECS/Entities/Entities.hpp"

using namespace game;
using entity_t = std::size_t;

Game::Game() :
    _manager(Loader()),
    _net(Network()),
    _roomId(0),
    _playerId(0),
    _startTimeLeft(0),
    _started(0)
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    this->_screenSize = {mode.width, mode.height};
    this->_window.create(sf::VideoMode(mode.width, mode.height), "R-TYPE");
    this->_window.setFramerateLimit(120);
    this->_lastTime = NOW;
    this->_net.setInst(9);
    this->_net.send();

    this->_manager.loadTexture("./client/assets/parallax/background.png", Loader::toLoad::ParallaxFirstbkg);
    this->_manager.loadTexture("./client/assets/parallax/background2.png", Loader::toLoad::ParallaxSecondbkg);

    // register components
    ecs.register_component<ECS::components::SpriteComponent>();
    ecs.register_component<ECS::components::PositionComponent>();
    ecs.register_component<ECS::components::TextureRectComponent>();
    ecs.register_component<ECS::components::VelocityComponent>();
    ecs.register_component<ECS::components::ControllableComponent>();
    ecs.register_component<ECS::components::ParallaxComponent>();
    ecs.register_component<ECS::components::MovableComponent>();

    // create parallaxFirst entity
    entitiesType::ParallaxEntity::create(this->ecs,
        this->_manager.getTexture(Loader::Loader::ParallaxFirstbkg),
        ECS::components::PositionComponent{0.0f, 0.0f},
        -0.15f);

    // create parallaxScd entity
    entitiesType::ParallaxEntity::create(this->ecs,
        this->_manager.getTexture(Loader::Loader::ParallaxSecondbkg),
        ECS::components::PositionComponent{0.0f, 0.0f},
        -0.20f);
}

Game::~Game()
{
}

entity_t Game::getEntityFromId(unsigned char id)
{
    for (auto &player : this->_players) {
        if (player.first == id)
            return player.second;
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
            this->_manager.loadTexture("./client/assets/entity/player/move.png", Loader::toLoad::Player);

            entity_t newEntity = ecs.spawn_entity();
            ecs.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{ -1000.0f, -1000.0f });
            ecs.emplace_component<ECS::components::MovableComponent>(newEntity, ECS::components::MovableComponent{});
            const sf::Texture &tmp =  this->_manager.getTexture(Loader::Loader::Player);
            ecs.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{ 0, 0, (int)tmp.getSize().x, (int)tmp.getSize().y, 5, 150.0f });
            ecs.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{ tmp });
            this->_players.push_back(std::make_pair(this->_playerId, newEntity));

            this->_manager.loadTexture("./client/assets/entity/rocket.png", Loader::toLoad::Rocket);
            this->_manager.loadTexture("./client/assets/entity/monsters/monster1.png", Loader::toLoad::Monster1);
        }

        if (packet.getInstruction() == 11) {
            this->_startTimeLeft = packet.getData().getDataUInt();
            this->_started = packet.getData().getDataUChar();
        }

        if (packet.getInstruction() == 3) {
            unsigned char id = packet.getData().getDataUChar();
            unsigned short x = packet.getData().getDataUShort();
            unsigned short y = packet.getData().getDataUShort();
            this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(this->getEntityFromId(id), x, y));
        }

        if (packet.getInstruction() == 5) {
            unsigned char id = packet.getData().getDataUChar();
            unsigned short x = packet.getData().getDataUShort();
            unsigned short y = packet.getData().getDataUShort();
            this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(this->getEntityFromId(id), x, y));
            entity_t newEntity = ecs.spawn_entity();
            ecs.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{ (float)x, (float)y });
            const sf::Texture &tmp =  this->_manager.getTexture(Loader::Loader::Rocket);
            ecs.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{ tmp });
            ecs.emplace_component<ECS::components::ParallaxComponent>(newEntity, ECS::components::ParallaxComponent{ 0.4f, (float)tmp.getSize().x });

        }
    }
    this->_net.setInst(12);
    this->_net.send();
}

int Game::MainLoop()
{
    while (this->_window.isOpen()) {
        long currentTime = NOW;
        float deltaTime = (currentTime - _lastTime) / 1.0f;
        _lastTime = currentTime;
        this->update();
        this->EventLoop(this->_window);
        // ALL SYSTEMS CALL
        ECS::systems::ControllableSystem().update(this->ecs, deltaTime);
        ECS::systems::PositionSystem().update(this->ecs);
        ECS::systems::AnimationSystem().update(this->ecs, deltaTime);
        ECS::systems::ParallaxSystem().update(this->ecs, deltaTime);
        ECS::systems::MovableSystem().update(this->ecs, this->_entityPositions);
        this->_window.clear();
        // DRAW SYSTEM CALL
        ECS::systems::DrawSystem().update(this->ecs, this->_window);
        this->_window.display();
    }
    return 0;
}
