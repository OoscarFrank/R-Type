#include <SFML/Window/Keyboard.hpp>
#include "../Registry.hpp"
#include "../Components/Components.hpp"

#define UP 1
#define DOWN 2
#define LEFT 4
#define RIGHT 8

namespace ECS {
    namespace systems {
        class ControllableSystem {
        public:
            class EntityMove {
                private:
                    entity_t entity;
                    char move;
                public:
                    EntityMove(entity_t entity) : entity(entity) {}
                    ~EntityMove() {}
                    void setMove(char move) {
                        this->move = move;
                    }
                    char getMove() {
                        return move;
                    }
                    void setEntity(entity_t entity) {
                        this->entity = entity;
                    }
                    entity_t getEntity() {
                        return entity;
                    }
            };

            ControllableSystem() = default;

            void update(Registry &ecs, std::vector<EntityMove> &entityMoves) {
                auto &ControllableComponents = ecs.get_components<components::ControllableComponent>();

                for (size_t i = 0; i < ControllableComponents.size(); ++i) {
                    auto &ControllableComponent = ControllableComponents[i];
                    if (!ControllableComponent)
                        continue;
                    entity_t entity = ecs.entity_from_index(i);
                    EntityMove entityMoveTmp(entity);
                    char move = 0;
                    for (auto key : ControllableComponent->getControls()) {
                        if (sf::Keyboard::isKeyPressed(key)) {
                            switch (key) {
                                case sf::Keyboard::Up:
                                    move |= UP;
                                    break;
                                case sf::Keyboard::Down:
                                    move |= DOWN;
                                    break;
                                case sf::Keyboard::Left:
                                    move |= LEFT;
                                    break;
                                case sf::Keyboard::Right:
                                    move |= RIGHT;
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                    if (move != 0) {
                        entityMoveTmp.setMove(move);
                        entityMoves.push_back(entityMoveTmp);
                    }
                }
            }
        };
    }
}
