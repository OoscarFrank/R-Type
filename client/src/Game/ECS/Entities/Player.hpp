#pragma once

#include "../Loader/Loader.hpp"
#include "../Registry.hpp"

namespace game {
    namespace entitiesType {
        class PlayerEntity {
            public:
                PlayerEntity() = default;

                static void newPlayer(ECS::Registry &ecs, const sf::Texture &texture,
                        const ECS::components::PositionComponent &positionComponent,
                        const ECS::components::VelocityComponent &velocityComponent,
                        const ECS::components::ControllableComponent &ControllableComponent,
                        int numFrames);
                ~PlayerEntity() {};

            private:
        };
    }
}
