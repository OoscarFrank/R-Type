#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "../../Error/MyError.hpp"

namespace game {
    namespace loader {
        class Loader {
            public:
                Loader();
                ~Loader();
                enum toLoad {
                    ParallaxFirstbkg,
                    ParallaxSecondbkg,
                    Player
                };
                void loadTexture(const std::string path, toLoad type);
                const sf::Texture &getTexture(toLoad type) const;

            private:
                std::unordered_map<Loader::toLoad, std::unique_ptr<sf::Texture>> _textures;
        };
    }
}
