#pragma once

#include "../Loader/Loader.hpp"
#include "../Registry.hpp"

namespace game {
    namespace entitiesType {
        class MissileEntity {
            public:
                MissileEntity() = default;

                static void create(ECS::Registry &ecs, const sf::Texture &texture,
                    const ECS::components::PositionComponent &positionComponent,
                    float scrollSpeed);
                ~MissileEntity() {};

            private:
        };
    }
}
