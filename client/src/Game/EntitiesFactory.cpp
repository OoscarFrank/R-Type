#include "EntitiesFactory.hpp"

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
    this->_registry.register_component<ECS::components::ScaleComponent>();
    this->_registry.register_component<ECS::components::ButtonComponent>();
    this->_registry.register_component<ECS::components::AnimationComponent>();
}

Factory::~Factory()
{
}

entity_t Factory::createButton(float x, float y, const std::shared_ptr<sf::Texture> &texture)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::ButtonComponent>(newEntity, ECS::components::ButtonComponent{});
    _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{0, 0, (int)texture->getSize().x, (int)texture->getSize().y, 2, 0.0f});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    return newEntity;
}

entity_t Factory::createLooserScreen(float x, float y, const std::shared_ptr<sf::Texture> &texture)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{0, 0, (int)texture->getSize().x, (int)texture->getSize().y, 1, 0.0f});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    return newEntity;
}

entity_t Factory::createPlayer(float x, float y, const std::shared_ptr<sf::Texture> &texture)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::MovableComponent>(newEntity, ECS::components::MovableComponent{});
    _registry.emplace_component<ECS::components::ControllableComponent>(newEntity, ECS::components::ControllableComponent{sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Space});
    _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{0, 0, (int)texture->getSize().x, (int)texture->getSize().y, 5, 150.0f});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    _registry.emplace_component<ECS::components::AnimationComponent>(newEntity, ECS::components::AnimationComponent{});
    // _registry.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{0.5, 0.5});

    return newEntity;
}

entity_t Factory::createParallax(float x, float y, const std::shared_ptr<sf::Texture> &texture, float scrollSpeed)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::ParallaxComponent>(newEntity, ECS::components::ParallaxComponent{scrollSpeed, (float)texture->getSize().x});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    return newEntity;
}

entity_t Factory::createMissile(float x, float y, const std::shared_ptr<sf::Texture> &texture)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::MovableComponent>(newEntity, ECS::components::MovableComponent{});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    // _registry.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{8.0, 8.0});
    return newEntity;
}

entity_t Factory::createEnnemi(float x, float y, const std::shared_ptr<sf::Texture> &texture)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::MovableComponent>(newEntity, ECS::components::MovableComponent{});
    _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{0, 0, (int)texture->getSize().x, (int)texture->getSize().y, 6, 200.0f});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    _registry.emplace_component<ECS::components::AnimationComponent>(newEntity, ECS::components::AnimationComponent{});
    return newEntity;
}

entity_t Factory::createBlackband(sf::IntRect rect, const std::shared_ptr<sf::Texture> &texture)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{rect.left, rect.top});
    // _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{0, 0, 1, 1, 1, 2.0f});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    _registry.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{rect.width, rect.height});
    return newEntity;
}