#pragma once

#include "ECS/Components/Components.hpp"
#include "ECS/Registry.hpp"

namespace game {
    class Factory {
        public:
        /**
         * @brief Construct a new Factory object
         *
         */
            Factory(ECS::Registry &registry);
        /**
         * @brief Destroy the Factory object
         *
         */
            ~Factory();
            entity_t createButton(float x, float y, const sf::Texture &texture);
        /**
         * @brief Create a Player object
         *
         * @param x
         * @param y
         * @param texture
         * @return entity_t
         */
            entity_t createPlayer(float x, float y, const sf::Texture &texture);
        /**
         * @brief Create a Parallax object
         *
         * @param x
         * @param y
         * @param texture
         * @param scrollSpeed
         * @return entity_t
         */
            entity_t createParallax(float x, float y, const sf::Texture &texture, float scrollSpeed);
        /**
         * @brief Create a Missile object
         *
         * @param x
         * @param y
         * @param texture
         * @return entity_t
         */
            entity_t createMissile(float x, float y, const sf::Texture &texture);
        /**
         * @brief Create a Ennemi object
         *
         * @param x
         * @param y
         * @param texture
         * @return entity_t
         */
            entity_t createEnnemi(float x, float y, const sf::Texture &texture);
        private:
            ECS::Registry &_registry;
    };
}