#pragma once

#include "../Registry.hpp"
#include "../Components/Components.hpp"
#include "../../Game/Fbr.hpp"

namespace ECS {
    namespace systems {
        class ParticuleSystem {
        public:
        /**
         * @brief Update the position of each entity with a position and a particule component
         *
         * @param ecs
         * @param particules
         * @param fbr
         * @param lastParticleUpdate
         */
            void update(Registry &ecs, std::vector<entity_t> &particules, std::unique_ptr<Fbr> &fbr, std::chrono::system_clock::time_point &lastParticleUpdate) {
                try {
                    for (size_t i = 0; i < particules.size(); ++i) {
                        if (ecs.isEntityExist(particules[i]) == false)
                            continue;
                        auto &particule = ecs.getComponent<components::ParticuleComponent>(particules[i]);

                        if ((std::chrono::system_clock::now() - particule.getTime()) > std::chrono::milliseconds(particule.getDuration())) {
                            ecs.kill_entity(particules[i]);
                            particules.erase(particules.begin() + i);
                        }

                        sf::IntRect area = particule.getRect();
                        sf::Color color = particule.getColor();

                        for (int i = 0; i < area.width * area.height / 1000; ++i) {
                            short x = std::rand() % area.width + area.left;
                            short y = std::rand() % area.height + area.top;
                            short width = std::rand() % 8 + 3;
                            short height = std::rand() % 8 + 3;
                            fbr->drawRect(x, y, width, height, color);
                        }

                    }
                    lastParticleUpdate = std::chrono::system_clock::now();
                } catch (std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        };
    }
}
