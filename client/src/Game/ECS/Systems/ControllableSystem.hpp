#include <SFML/Window/Keyboard.hpp>
#include "../Registry.hpp"
#include "../Components/Components.hpp"

#define UP 1
#define DOWN 2
#define LEFT 4
#define RIGHT 8

namespace ECS
{
    namespace systems
    {
        class ControllableSystem
        {
        public:
            class EntityMove
            {
            private:
                entity_t entity;
                char move;
            public:
                /**
                 * @brief Construct a new Entity Move object
                 *
                 * @param entity
                 */
                EntityMove(entity_t entity) : entity(entity) {}
                /**
                 * @brief Destroy the Entity Move object
                 *
                 */
                ~EntityMove() {}
                /**
                 * @brief Set the Move object
                 *
                 * @param move
                 */
                void setMove(char move)
                {
                    this->move = move;
                }
                /**
                 * @brief Get the Move object
                 *
                 * @return char
                 */
                char getMove()
                {
                    return move;
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

            ControllableSystem() = default;
            /**
             * @brief Update controls of each entity with a controllable component
             *
             * @param ecs
             * @param entityMoves
             */
            void update(Registry &ecs, std::vector<EntityMove> &entityMoves, sf::RenderWindow &window, unsigned char &move)
            {
                try
                {
                    auto &ControllableComponents = ecs.get_components<components::ControllableComponent>();
                    sf::Event event;

                    
                    while (window.pollEvent(event))
                    {
                        if (event.type == sf::Event::Closed)
                        {
                            window.close();
                            return;
                        }
                        if (event.type == sf::Event::KeyPressed)
                        {
                            switch (event.key.code)
                            {
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
                        if (event.type == sf::Event::KeyReleased)
                        {
                            switch (event.key.code)
                            {
                            case sf::Keyboard::Up:
                                move ^= UP;
                                break;
                            case sf::Keyboard::Down:
                                move ^= DOWN;
                                break;
                            case sf::Keyboard::Left:
                                move ^= LEFT;
                                break;
                            case sf::Keyboard::Right:
                                move ^= RIGHT;
                                break;
                            default:
                                break;
                            }
                        }
                    }

                    for (size_t i = 0; i < ControllableComponents.size(); ++i)
                    {
                        auto &ControllableComponent = ControllableComponents[i];
                        if (!ControllableComponent)
                            continue;
                        unsigned char p_move = ControllableComponent->getMove();
                        for (auto &key : ControllableComponent->getControls()) {
                            
                            switch (key) {
                                case sf::Keyboard::Up:
                                    if (move & UP && !(p_move & UP))
                                        ControllableComponent->setMove(p_move | UP);
                                    else if (!(move & UP) && p_move & UP)
                                        ControllableComponent->setMove(p_move ^ UP);
                                    break;
                                case sf::Keyboard::Down:
                                    if (move & DOWN && !(p_move & DOWN))
                                        ControllableComponent->setMove(p_move | DOWN);
                                    else if (!(move & DOWN) && p_move & DOWN)
                                        ControllableComponent->setMove(p_move ^ DOWN);
                                    break;
                                case sf::Keyboard::Left:
                                    if (move & LEFT && !(p_move & LEFT))
                                        ControllableComponent->setMove(p_move | LEFT);
                                    else if (!(move & LEFT) && p_move & LEFT)
                                        ControllableComponent->setMove(p_move ^ LEFT);
                                    break;
                                case sf::Keyboard::Right:
                                    if (move & RIGHT && !(p_move & RIGHT))
                                        ControllableComponent->setMove(p_move | RIGHT);
                                    else if (!(move & RIGHT) && p_move & RIGHT)
                                        ControllableComponent->setMove(p_move ^ RIGHT);
                                    break;
                                default:
                                    break;
                            }
                            
                        }
                        entity_t entity = ecs.entity_from_index(i);
                        EntityMove entityMoveTmp(entity);

                        if (ControllableComponent->getMove() != 0)
                        {
                            entityMoveTmp.setMove(ControllableComponent->getMove());
                            entityMoves.push_back(entityMoveTmp);
                        }
                    }
                
                }
                catch (std::exception &e)
                {
                    std::cerr << e.what() << std::endl;
                }
            
            }
        };
    }
}
