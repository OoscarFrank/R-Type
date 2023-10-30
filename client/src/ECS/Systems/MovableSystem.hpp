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
            /**
             * @brief Construct a new Entity Pos object
             *
             * @param entity
             * @param x
             * @param y
             */
                EntityPos(entity_t entity, float x, float y) : entity(entity), x(x), y(y) {}
            /**
             * @brief Get the X position
             *
             * @return float
             */
                float getX() const { return x; }
            /**
             * @brief Get the Y position
             *
             * @return float
             */
                float getY() const { return y; }
            /**
             * @brief Get the Entity object
             *
             * @return entity_t
             */
                entity_t getEntity() const { return entity; }
            };

            MovableSystem() = default;
            /**
             * @brief Update the position of each entity with a position and a movable component
             *
             * @param ecs
             * @param entityPositions
             */
            void update(Registry &ecs, std::vector<EntityPos> &entityPositions, sf::Vector2u offset = {0, 0}) {

                for (auto i = entityPositions.begin(); i != entityPositions.end(); ++i) {
                    try {
                        if (ecs.hasComponent<ECS::components::PositionComponent>(i->getEntity())) {
                            ECS::components::PositionComponent &component = ecs.getComponent<ECS::components::PositionComponent>(i->getEntity());
                            if (!ecs.hasComponent<ECS::components::MovableComponent>(i->getEntity()))
                                continue;
                            component.setX(i->getX() + offset.x);
                            component.setY(i->getY() + offset.y);
                        }
                    } catch (std::exception &e) {
                    }
                }
                entityPositions.clear();
            }
        };
    }
}
