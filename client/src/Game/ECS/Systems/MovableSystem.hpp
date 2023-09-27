#include <SFML/Window/Keyboard.hpp>
#include "../Registry.hpp"
#include "../Components/Components.hpp"

namespace ECS {
    namespace systems {
        class MovableSystem {
        public:
            class EntityPos {
            private:
                entity_t entity;
                float x;
                float y;
            public:
                EntityPos(entity_t entity, float x, float y) : entity(entity), x(x), y(y) {}
                float getX() const { return x; }
                float getY() const { return y; }
                entity_t getEntity() const { return entity; }
            };

            MovableSystem() = default;

            void update(Registry &ecs, std::vector<EntityPos> &entityPositions) {
                for (auto i = entityPositions.begin(); i != entityPositions.end(); ++i) {
                    ECS::components::PositionComponent &component = ecs.getComponent<ECS::components::PositionComponent>(i->getEntity());
                    component.setX(i->getX());
                    component.setY(i->getY());
                }

                entityPositions.clear();
            }
        };
    }
}
