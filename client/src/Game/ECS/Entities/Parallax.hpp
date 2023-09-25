#pragma once

#include "../Loader/Loader.hpp"
#include "../Registry.hpp"

namespace game {
    namespace entitiesType {
        class ParallaxEntity {
            public:
                ParallaxEntity() = default;

                static void newParallax(ECS::Registry &ecs, const sf::Texture &texture,
                    const ECS::components::PositionComponent &positionComponent,
                    float scrollSpeed);
                ~ParallaxEntity() {};

            private:
        };
    }
}
