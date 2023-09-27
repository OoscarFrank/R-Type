#include <SFML/Window/Keyboard.hpp>
#include "../Registry.hpp"
#include "../Components/Components.hpp"

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
                    entity_t entity = ecs.entity_from_index(i);
                    EntityMove entityMoveTmp(entity);
                    for (auto key : ControllableComponent->getControls()) {
                        if (sf::Keyboard::isKeyPressed(key)) {
                            switch (key) {
                                case sf::Keyboard::Up:
                                    entityMoveTmp.setMove(1);
                                    break;
                                case sf::Keyboard::Down:
                                    entityMoveTmp.setMove(2);
                                    break;
                                case sf::Keyboard::Left:
                                    entityMoveTmp.setMove(3);
                                    break;
                                case sf::Keyboard::Right:
                                    entityMoveTmp.setMove(4);
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                    entityMoves.push_back(entityMoveTmp);
                }
            }
        };
    }
}
