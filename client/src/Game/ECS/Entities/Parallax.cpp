#include "Parallax.hpp"

using namespace game::entitiesType;

void ParallaxEntity::create(ECS::Registry &ecs, const sf::Texture &texture,
        const ECS::components::PositionComponent &positionComponent,
        float scrollSpeed) {
    entity_t newEntity = ecs.spawn_entity();
    ecs.emplace_component<ECS::components::SpriteComponent>(newEntity, ECS::components::SpriteComponent{ texture });
    ecs.emplace_component<ECS::components::ParallaxComponent>(newEntity, ECS::components::ParallaxComponent{ scrollSpeed, (float)texture.getSize().x });
    ecs.emplace_component<ECS::components::PositionComponent>(newEntity, positionComponent);
}
