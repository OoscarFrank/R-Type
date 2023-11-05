#pragma once

#include "../Registry.hpp"
#include "../Components/Components.hpp"

namespace ECS {
    namespace systems {
        class SoundSystem {
        public:
        /**
         * @brief Update the sound system
         *
         * @param ecs
         * @param deltaTime
         */
            void update(Registry &ecs, std::vector<entity_t> &sounds) {
                try {
                    for (size_t i = 0; i < sounds.size(); ++i) {
                        if (ecs.isEntityExist(sounds[i]) == false)
                            continue;
                        auto &sound = ecs.getComponent<components::SoundComponent>(sounds[i]);
                        bool isPlaying = sound.getStatus() == sf::Sound::Playing ? true : false;

                        if (isPlaying == false) {
                            ecs.kill_entity(sounds[i]);
                            sounds.erase(sounds.begin() + i);
                        }
                    }
                } catch (std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        };
    }
}
