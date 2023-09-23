#include "Game.hpp"
#include "ECS/Components/Components.hpp"
#include "ECS/Systems/Systems.hpp"

using namespace game;
using entity_t = std::size_t;

Game::Game() :
    _manager(Loader())
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    this->_screenSize = {mode.width, mode.height};
    this->_window.create(sf::VideoMode(mode.width, mode.height), "R-TYPE");
    this->_lastTime = NOW;

    // register components
    ecs.register_component<ECS::components::SpriteComponent>();
    ecs.register_component<ECS::components::PositionComponent>();
    ecs.register_component<ECS::components::TextureRectComponent>();
    ecs.register_component<ECS::components::VelocityComponent>();

    // create test entity
    entity_t playerEntity = ecs.spawn_entity();
    ecs.emplace_component<ECS::components::PositionComponent>(playerEntity, ECS::components::PositionComponent{ 100.0f, 100.0f });
    ecs.emplace_component<ECS::components::VelocityComponent>(playerEntity, ECS::components::VelocityComponent{ 0.0f, 0.0f });

    const sf::Texture &texture = this->_manager.getTexture(Loader::Loader::Player);
    ecs.emplace_component<ECS::components::TextureRectComponent>(playerEntity, ECS::components::TextureRectComponent{ 0, 0, (int)texture.getSize().x, (int)texture.getSize().y, 5, 150.0f });
    ecs.emplace_component<ECS::components::SpriteComponent>(playerEntity, ECS::components::SpriteComponent{ texture });
}

Game::~Game()
{
}

int Game::MainLoop()
{
    while (this->_window.isOpen()) {
        long currentTime = NOW;
        float deltaTime = (currentTime - _lastTime) / 1.0f;
        _lastTime = currentTime;

        this->EventLoop(this->_window);
        // ALL SYSTEMS CALL
        ECS::systems::PositionSystem().update(this->ecs);
        ECS::systems::AnimationSystem().update(this->ecs, deltaTime);
        this->_window.clear();
        // DRAW SYSTEM CALL
        ECS::systems::DrawSystem().update(this->ecs, this->_window);
        this->_window.display();
    }
    return 0;
}
