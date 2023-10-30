#pragma once

#include "../Registry.hpp"
#include "../Components/Components.hpp"
#include <unordered_map>

namespace ECS {
    namespace systems {
        class TextSystem {
            public:
                void update(Registry &ecs, std::unordered_map<entity_t, std::string> &texts) {
                    try {
                        auto &textComponents = ecs.get_components<components::TextComponent>();
                        for (size_t i = 0; i < textComponents.size(); ++i) {
                            auto &text = textComponents[i];
                            if (text) {
                                for(auto j = texts.begin(); j != texts.end(); ++j) {
                                    if (i == (*j).first) {
                                        text->setString((*j).second);
                                    }
                                }
                            }
                        }
                        texts.clear();
                    } catch (std::exception &e) {
                        std::cerr << e.what() << std::endl;
                    }
                }
        };
    }
}
