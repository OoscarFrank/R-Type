#include "Player.hpp"

using namespace game::entitiesType;

void PlayerEntity::create(ECS::Registry &ecs, const sf::Texture &texture,
        const ECS::components::PositionComponent &positionComponent,
        const ECS::components::VelocityComponent &velocityComponent,
        const ECS::components::ControllableComponent &ControllableComponent,
        int numFrames) {
    entity_t newEntity = ecs.spawn_entity();
    ecs.emplace_component<ECS::components::PositionComponent>(newEntity, positionComponent);
    ecs.emplace_component<ECS::components::VelocityComponent>(newEntity, velocityComponent);
    ecs.emplace_component<ECS::components::ControllableComponent>(newEntity, ControllableComponent);

    ecs.emplace_component<ECS::components::TextureRectComponent>(newEntity, ECS::components::TextureRectComponent{ 0, 0, (int)texture.getSize().x, (int)texture.getSize().y, numFrames, 150.0f });
    ecs.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{ texture });
}
