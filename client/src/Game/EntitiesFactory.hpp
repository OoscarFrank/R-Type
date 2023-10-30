#pragma once

#include "../ECS/Components/Components.hpp"
#include "../ECS/Registry.hpp"
#include <memory>

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
            entity_t createButton(float x, float y, const std::shared_ptr<sf::Texture> &texture, const sf::Vector2f &scale, std::function<void()> callback);
            entity_t createLooserScreen(float x, float y, const std::shared_ptr<sf::Texture> &texture);
        /**
         * @brief Create a Player object
         *
         * @param x
         * @param y
         * @param texture
         * @return entity_t
         */
            entity_t createPlayer(float x, float y, const std::shared_ptr<sf::Texture> &texture);
        /**
         * @brief Create a Parallax object
         *
         * @param x
         * @param y
         * @param texture
         * @param scrollSpeed
         * @return entity_t
         */
            entity_t createParallax(float x, float y, const std::shared_ptr<sf::Texture> &texture, float scrollSpeed, const sf::Vector2f &scale, float ratio = 1.0f);
        /**
         * @brief Create a Missile object
         *
         * @param x
         * @param y
         * @param texture
         * @return entity_t
         */
            entity_t createMissile(float x, float y, const std::shared_ptr<sf::Texture> &texture);
        /**
         * @brief Create a Ennemi object
         *
         * @param x
         * @param y
         * @param texture
         * @return entity_t
         */
            entity_t createEnnemi(float x, float y, const std::shared_ptr<sf::Texture> &texture);
            entity_t createEnnemi4frames(float x, float y, const std::shared_ptr<sf::Texture> &texture, float scale);
            entity_t createBlackband(sf::IntRect rect, const std::shared_ptr<sf::Texture> &texture);

            entity_t createScoreCoche(float x, float y, const std::shared_ptr<sf::Texture> &texture, float scale);

            entity_t createMusic(const std::string &musicPath, float volume = 100.0f, bool loop = false);
            entity_t createStrobe(const std::shared_ptr<sf::Texture> &texture, float x, float y);

            entity_t createText(const std::string &text, const std::shared_ptr<sf::Font> &font, const float &x = 0, const float &y = 0, const size_t &size = 20, const sf::Color &color = sf::Color::White, const sf::Text::Style &style = sf::Text::Style::Regular);
            entity_t createLoadingBar(float x, float y, const std::shared_ptr<sf::Texture> &texture, const std::shared_ptr<sf::Texture> &textureRect, float scale);
            entity_t createSound(const std::string &soundPath, float volume = 100.0f);
        private:
            ECS::Registry &_registry;
    };
}