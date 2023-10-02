#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../../../Error/MyError.hpp"

namespace game {
    class Loader {
        public:
        /**
         * @brief Construct a new Loader object
         *
         */
            Loader();
        /**
         * @brief Destroy the Loader object
         *
         */
            ~Loader();
            enum toLoad {
                ParallaxFirstbkg,
                ParallaxSecondbkg,
                Player_move1, Player_move2, Player_move3, Player_move4,
                Rocket,
                Monster1
            };
        /**
         * @brief Load a texture from a path
         *
         * @param path
         * @param type
         */
            void loadTexture(const std::string path, toLoad type);
        /**
         * @brief Unload a texture from a type
         *
         * @param type
         */
            void unloadTexture(toLoad type);
        /**
         * @brief Get the Texture object
         *
         * @param type
         * @return const sf::Texture&
         */
            const sf::Texture &getTexture(toLoad type) const;

        private:
            std::unordered_map<Loader::toLoad, std::unique_ptr<sf::Texture>> _textures;
    };
}
