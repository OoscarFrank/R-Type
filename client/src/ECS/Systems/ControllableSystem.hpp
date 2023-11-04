#include <SFML/Window/Keyboard.hpp>
#include "../Registry.hpp"
#include "../Components/Components.hpp"

#define UP 1
#define DOWN 2
#define LEFT 4
#define RIGHT 8
#define SPACE 16
#define ENTER 32
#define BOMB 64
#define LASER 128
#define RAY 256

namespace ECS {
    namespace systems {
        class ControllableSystem {
            public:
                class EntityEvent {
                    private:
                    /**
                     * @brief Entity
                     * 
                     */
                        entity_t entity;
                    /**
                     * @brief Event
                     * 
                     */
                        int event = 0;
                    public:
                        /**
                         * @brief Construct a new Entity Move object
                         *
                         * @param entity
                         */
                        EntityEvent(entity_t entity) : entity(entity) {}
                        /**
                         * @brief Destroy the Entity Move object
                         *
                         */
                        ~EntityEvent() {}
                        /**
                         * @brief Set the Move object
                         *
                         * @param move
                         */
                        void setEvent(int event)
                        {
                            this->event = event;
                        }
                        /**
                         * @brief Get the Move object
                         *
                         * @return int
                         */
                        int getEvent()
                        {
                            return event;
                        }
                        /**
                         * @brief Set the Entity object
                         *
                         * @param entity
                         */
                        void setEntity(entity_t entity)
                        {
                            this->entity = entity;
                        }
                        /**
                         * @brief Get the Entity object
                         *
                         * @return entity_t
                         */
                        entity_t getEntity() const
                        {
                            return entity;
                        }
                };
                /**
                 * @brief Construct a new Controllable System object
                 * 
                 */
                ControllableSystem() = default;
                /**
                 * @brief Update controls of each entity with a controllable component
                 *
                 * @param ecs
                 * @param entityMoves
                 */
                void update(Registry &ecs, std::vector<EntityEvent> &entityMoves, std::vector<u_char> &keyboardEntries, sf::RenderWindow &window, int &eventMemory) {
                    try {
                        auto &ControllableComponents = ecs.get_components<components::ControllableComponent>();
                        sf::Event event;
                        while (window.pollEvent(event)) {
                            if (event.type == sf::Event::Closed) {
                                window.close();
                                return;
                            }
                            if (event.type == sf::Event::KeyPressed) {
                                switch (event.key.code)
                                {
                                case sf::Keyboard::Escape:
                                    window.close();
                                    return;
                                case sf::Keyboard::Up:
                                    eventMemory |= UP;
                                    break;
                                case sf::Keyboard::Down:
                                    eventMemory |= DOWN;
                                    break;
                                case sf::Keyboard::Left:
                                    eventMemory |= LEFT;
                                    break;
                                case sf::Keyboard::Right:
                                    eventMemory |= RIGHT;
                                    break;
                                case sf::Keyboard::Space:
                                    eventMemory |= SPACE;
                                    break;
                                case sf::Keyboard::Enter:
                                    eventMemory |= ENTER;
                                    break;
                                case sf::Keyboard::A:
                                    eventMemory |= BOMB;
                                    break;
                                case sf::Keyboard::E:
                                    eventMemory |= LASER;
                                    break;
                                case sf::Keyboard::Z:
                                    eventMemory |= RAY;
                                    break;
                                default:
                                    break;
                                }
                            }
                            if (event.type == sf::Event::KeyReleased) {
                                switch (event.key.code)
                                {
                                case sf::Keyboard::Up:
                                    eventMemory ^= UP;
                                    break;
                                case sf::Keyboard::Down:
                                    eventMemory ^= DOWN;
                                    break;
                                case sf::Keyboard::Left:
                                    eventMemory ^= LEFT;
                                    break;
                                case sf::Keyboard::Right:
                                    eventMemory ^= RIGHT;
                                    break;
                                case sf::Keyboard::Space:
                                    eventMemory ^= SPACE;
                                    break;
                                case sf::Keyboard::Enter:
                                    eventMemory ^= ENTER;
                                    break;
                                case sf::Keyboard::A:
                                    eventMemory ^= BOMB;
                                    break;
                                case sf::Keyboard::E:
                                    eventMemory ^= LASER;
                                    break;
                                case sf::Keyboard::Z:
                                    eventMemory ^= RAY;
                                    break;
                                default:
                                    break;
                                }
                            }
                            if (event.type == sf::Event::TextEntered && event.text.unicode < 128)
                                keyboardEntries.push_back(static_cast<u_char>(event.text.unicode));
                        }

                        for (size_t i = 0; i < ControllableComponents.size(); ++i)
                        {
                            auto &ControllableComponent = ControllableComponents[i];
                            if (!ControllableComponent)
                                continue;
                            int p_move = ControllableComponent->getEvent();
                            for (auto &key : ControllableComponent->getControls()) {
                                switch (key) {
                                    case sf::Keyboard::Up:
                                        if (+eventMemory & UP && !(p_move & UP))
                                            ControllableComponent->setEvent(p_move | UP);
                                        else if (!(eventMemory & UP) && p_move & UP)
                                            ControllableComponent->setEvent(p_move ^ UP);
                                        break;
                                    case sf::Keyboard::Down:
                                        if (eventMemory & DOWN && !(p_move & DOWN))
                                            ControllableComponent->setEvent(p_move | DOWN);
                                        else if (!(eventMemory & DOWN) && p_move & DOWN)
                                            ControllableComponent->setEvent(p_move ^ DOWN);
                                        break;
                                    case sf::Keyboard::Left:
                                        if (eventMemory & LEFT && !(p_move & LEFT))
                                            ControllableComponent->setEvent(p_move | LEFT);
                                        else if (!(eventMemory & LEFT) && p_move & LEFT)
                                            ControllableComponent->setEvent(p_move ^ LEFT);
                                        break;
                                    case sf::Keyboard::Right:
                                        if (eventMemory & RIGHT && !(p_move & RIGHT))
                                            ControllableComponent->setEvent(p_move | RIGHT);
                                        else if (!(eventMemory & RIGHT) && p_move & RIGHT)
                                            ControllableComponent->setEvent(p_move ^ RIGHT);
                                        break;
                                    case sf::Keyboard::Space:
                                        if (eventMemory & SPACE && !(p_move & SPACE))
                                            ControllableComponent->setEvent(p_move | SPACE);
                                        else if (!(eventMemory & SPACE) && p_move & SPACE)
                                            ControllableComponent->setEvent(p_move ^ SPACE);
                                        break;
                                    case sf::Keyboard::Enter:
                                        if (eventMemory & ENTER && !(p_move & ENTER))
                                            ControllableComponent->setEvent(p_move | ENTER);
                                        else if (!(eventMemory & ENTER) && p_move & ENTER)
                                            ControllableComponent->setEvent(p_move ^ ENTER);
                                        break;
                                    case sf::Keyboard::A:
                                        if (eventMemory & BOMB && !(p_move & BOMB))
                                            ControllableComponent->setEvent(p_move | BOMB);
                                        else if (!(eventMemory & BOMB) && p_move & BOMB)
                                            ControllableComponent->setEvent(p_move ^ BOMB);
                                        break;
                                    case sf::Keyboard::E:
                                        if (eventMemory & LASER && !(p_move & LASER))
                                            ControllableComponent->setEvent(p_move | LASER);
                                        else if (!(eventMemory & LASER) && p_move & LASER)
                                            ControllableComponent->setEvent(p_move ^ LASER);
                                        break;
                                    case sf::Keyboard::Z:
                                        if (eventMemory & RAY && !(p_move & RAY))
                                            ControllableComponent->setEvent(p_move | RAY);
                                        else if (!(eventMemory & RAY) && p_move & RAY)
                                            ControllableComponent->setEvent(p_move ^ RAY);
                                        break;
                                    default:
                                        break;
                                }
                            }
                            if (ControllableComponent->getEvent() != 0) {
                                entity_t entity = ecs.entity_from_index(i);
                                EntityEvent entityMoveTmp(entity);
                                entityMoveTmp.setEvent(ControllableComponent->getEvent());
                                entityMoves.push_back(entityMoveTmp);
                            }
                        }

                    }
                    catch (std::exception &e) {
                        std::cerr << e.what() << std::endl;
                    }

                }
        };
    }
}
