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
    this->_registry.register_component<ECS::components::ScoreComponent>();
    this->_registry.register_component<ECS::components::MusicComponent>();
    this->_registry.register_component<ECS::components::TextComponent>();
    this->_registry.register_component<ECS::components::RectangleShapeComponent>();
    this->_registry.register_component<ECS::components::LoadingBarComponent>();
    this->_registry.register_component<ECS::components::SoundComponent>();
}

Factory::~Factory()
{
}

entity_t Factory::createButton(float x, float y, const std::shared_ptr<sf::Texture> &texture, const sf::Vector2f &scale, std::function<void()> callback)
{
    entity_t newEntity = _registry.spawn_entity(80, false);
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::ButtonComponent>(newEntity, ECS::components::ButtonComponent{callback});
    _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{0, 0, (int)texture->getSize().x, (int)texture->getSize().y, 2, 0.0f});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    _registry.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{scale.x, scale.y});
    return newEntity;
}

entity_t Factory::createLooserScreen(float x, float y, const std::shared_ptr<sf::Texture> &texture)
{
    entity_t newEntity = _registry.spawn_entity(80);
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{0, 0, (int)texture->getSize().x, (int)texture->getSize().y, 1, 0.0f});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    return newEntity;
}

entity_t Factory::createPlayer(float x, float y, const std::shared_ptr<sf::Texture> &texture)
{
    entity_t newEntity = _registry.spawn_entity(60);
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::MovableComponent>(newEntity, ECS::components::MovableComponent{});
    _registry.emplace_component<ECS::components::ControllableComponent>(newEntity, ECS::components::ControllableComponent{sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Space});
    _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{0, 0, (int)texture->getSize().x, (int)texture->getSize().y, 5, 150.0f});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    _registry.emplace_component<ECS::components::AnimationComponent>(newEntity, ECS::components::AnimationComponent{});

    return newEntity;
}

entity_t Factory::createParallax(float x, float y, const std::shared_ptr<sf::Texture> &texture, float scrollSpeed, const sf::Vector2f &scale, float ratio)
{
    entity_t newEntity = _registry.spawn_entity(20);
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::ParallaxComponent>(newEntity, ECS::components::ParallaxComponent{scrollSpeed, (float)(texture->getSize().x) * ratio});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    _registry.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{scale.x, scale.y});
    return newEntity;
}

entity_t Factory::createMissile(float x, float y, const std::shared_ptr<sf::Texture> &texture)
{
    entity_t newEntity = _registry.spawn_entity(59);
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::MovableComponent>(newEntity, ECS::components::MovableComponent{});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    return newEntity;
}

entity_t Factory::createEnnemi(float x, float y, const std::shared_ptr<sf::Texture> &texture)
{
    entity_t newEntity = _registry.spawn_entity(60);
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::MovableComponent>(newEntity, ECS::components::MovableComponent{});
    _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{0, 0, (int)texture->getSize().x, (int)texture->getSize().y, 6, 200.0f});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    _registry.emplace_component<ECS::components::AnimationComponent>(newEntity, ECS::components::AnimationComponent{});
    return newEntity;
}

entity_t Factory::createEnnemi4frames(float x, float y, const std::shared_ptr<sf::Texture> &texture, float scale)
{
    entity_t newEntity = _registry.spawn_entity(60);
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::MovableComponent>(newEntity, ECS::components::MovableComponent{});
    _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{0, 0, (int)texture->getSize().x, (int)texture->getSize().y, 4, 100.0f});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    _registry.emplace_component<ECS::components::AnimationComponent>(newEntity, ECS::components::AnimationComponent{});
    _registry.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{static_cast<float>(scale), static_cast<float>(scale)});
    return newEntity;
}

entity_t Factory::createBlackband(sf::IntRect rect, const std::shared_ptr<sf::Texture> &texture)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{static_cast<float>(rect.left), static_cast<float>(rect.top)});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    _registry.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{static_cast<float>(rect.width), static_cast<float>(rect.height)});
    return newEntity;
}

entity_t Factory::createScoreCoche(float x, float y, const std::shared_ptr<sf::Texture> &texture, float scale)
{
    entity_t newEntity = _registry.spawn_entity(90);
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::ScoreComponent>(newEntity, ECS::components::ScoreComponent{});
    _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{0, 0, (int)texture->getSize().x, (int)texture->getSize().y, 1, 0.0f});
    _registry.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{static_cast<float>(scale), static_cast<float>(scale)});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    return newEntity;
}

entity_t Factory::createMusic(const std::string &musicPath, float volume, bool loop)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::MusicComponent>(newEntity, ECS::components::MusicComponent{musicPath, volume, loop});
    return newEntity;
}

entity_t Factory::createStrobe(const std::shared_ptr<sf::Texture> &texture, float x, float y)
{
    entity_t newEntity = _registry.spawn_entity(100);
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    _registry.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{static_cast<float>(x), static_cast<float>(y)});
    return newEntity;
}

entity_t Factory::createText(const std::string &text, const std::shared_ptr<sf::Font> &font, const float &x, const float &y, const size_t &size, const sf::Color &color, const sf::Text::Style &style)
{
    entity_t newEntity = _registry.spawn_entity(100);
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::TextComponent>(newEntity, ECS::components::TextComponent{*font.get(), text, size, color, style});
    return newEntity;
}

entity_t Factory::createLoadingBar(float x, float y, const std::shared_ptr<sf::Texture> &texture, const std::shared_ptr<sf::Texture> &textureRect, float scale)
{
    entity_t newEntity = _registry.spawn_entity(90);
    _registry.emplace_component<ECS::components::PositionComponent>(newEntity, ECS::components::PositionComponent{x, y});
    _registry.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{(int)(texture->getSize().x - (texture->getSize().x / 1)), 0, (int)texture->getSize().x, (int)texture->getSize().y, 1, 0.0f});
    _registry.emplace_component<ECS::components::RectangleShapeComponent>(newEntity, ECS::components::RectangleShapeComponent{textureRect, sf::IntRect(0, 0, textureRect->getSize().x, textureRect->getSize().y), sf::Vector2f(static_cast<float>(x), static_cast<float>(y))});
    _registry.emplace_component<ECS::components::ScaleComponent>(newEntity, ECS::components::ScaleComponent{static_cast<float>(scale), static_cast<float>(scale)});
    _registry.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{texture});
    return newEntity;
}

entity_t Factory::createSound(const std::string &soundPath, float volume)
{
    entity_t newEntity = _registry.spawn_entity();
    _registry.emplace_component<ECS::components::SoundComponent>(newEntity, ECS::components::SoundComponent{soundPath, volume});
    return newEntity;
}
