#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../../../Error/MyError.hpp"

namespace game {
    class Loader {
        public:
            Loader();
            ~Loader();
            enum toLoad {
                ParallaxFirstbkg,
                ParallaxSecondbkg,
                Player,
                Rocket,
                Monster1
            };
            void loadTexture(const std::string path, toLoad type);
            const sf::Texture &getTexture(toLoad type) const;

        private:
            std::unordered_map<Loader::toLoad, std::unique_ptr<sf::Texture>> _textures;
    };
}
