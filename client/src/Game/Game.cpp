#include "./Game.hpp"
#include "./ECS/Components/Components.hpp"
#include "./ECS/Systems/Systems.hpp"
#include "./ECS/Entities/Entities.hpp"

using namespace game;
using entity_t = std::size_t;

Game::Game() :
    _manager(Loader()),
    _net(Network())
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    this->_screenSize = {mode.width, mode.height};
    this->_window.create(sf::VideoMode(mode.width, mode.height), "R-TYPE");
    this->_window.setFramerateLimit(120);
    this->_lastTime = NOW;
    this->_net.setInst(9);
    this->_net.send();

    // register components
    ecs.register_component<ECS::components::SpriteComponent>();
    ecs.register_component<ECS::components::PositionComponent>();
    ecs.register_component<ECS::components::TextureRectComponent>();
    ecs.register_component<ECS::components::VelocityComponent>();
    ecs.register_component<ECS::components::ControllableComponent>();
    ecs.register_component<ECS::components::ParallaxComponent>();

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

    // create player entity
    entitiesType::PlayerEntity::create(this->ecs,
        this->_manager.getTexture(Loader::Loader::Player),
        ECS::components::PositionComponent{ 100.0f, 100.0f },
        ECS::components::VelocityComponent{ 0.8f, 0.8f },
        ECS::components::ControllableComponent{ sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right }, 5);

    // create missile entity
    entitiesType::MissileEntity::create(this->ecs,
        this->_manager.getTexture(Loader::Loader::Rocket),
        ECS::components::PositionComponent{100.0f, 300.0f},
        0.4f);
}

Game::~Game()
{
}

void Game::update()
{
    Network::Packet packet;

    while (this->_net.getQueueIn().tryPop(packet)) {
        std::cout << "Packet received : " << packet.getData().getDataInt() << std::endl;
    }
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
        this->_window.clear();
        // DRAW SYSTEM CALL
        ECS::systems::DrawSystem().update(this->ecs, this->_window);
        this->_window.display();
    }
    return 0;
}
