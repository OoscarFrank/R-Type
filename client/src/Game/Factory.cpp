#include "Factory.hpp"

using namespace game;

Factory::Factory(ECS::Registry &registry): _registry(registry)
{
    // register components
    this->_registry.register_component<ECS::components::SpriteComponent>();
    this->_registry.register_component<ECS::components::PositionComponent>();
    this->_registry.register_component<ECS::components::TextureRectComponent>();
    this->_registry.register_component<ECS::components::VelocityComponent>();
    this->_registry.register_component<ECS::components::ControllableComponent>();
    this->_registry.register_component<ECS::components::ParallaxComponent>();
    this->_registry.register_component<ECS::components::MovableComponent>();
}

Factory::~Factory()
{
}

entity_t Factory::createPlayer(float x, float y, const sf::Texture &texture)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::MovableComponent>(newEntity, ECS::components::MovableComponent{});
    _registry.emplace_component<ECS::components::ControllableComponent>(newEntity, ECS::components::ControllableComponent{sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right});
    _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{0, 0, (int)texture.getSize().x, (int)texture.getSize().y, 5, 150.0f});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    return newEntity;
}

entity_t Factory::createParallax(float x, float y, const sf::Texture &texture, float scrollSpeed)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::ParallaxComponent>(newEntity, ECS::components::ParallaxComponent{scrollSpeed, (float)texture.getSize().x});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    return newEntity;
}

entity_t Factory::createMissile(float x, float y, const sf::Texture &texture)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::MovableComponent>(newEntity, ECS::components::MovableComponent{});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    return newEntity;
}

entity_t Factory::createEnnemie(float x, float y, const sf::Texture &texture)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::MovableComponent>(newEntity, ECS::components::MovableComponent{});
    _registry.emplace_component<ECS::components::ControllableComponent>(newEntity, ECS::components::ControllableComponent{sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right});
    _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{0, 0, (int)texture.getSize().x, (int)texture.getSize().y, 5, 150.0f});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    return newEntity;
}
