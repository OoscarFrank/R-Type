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
            entity_t createScreen(float x, float y, const std::shared_ptr<sf::Texture> &texture, const sf::Vector2f &scale = {1.0, 1.0});
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
        /**
         * @brief Create a Ennemi Frames object
         *
         * @param x
         * @param y
         * @param texture
         * @param scale
         * @param frame
         * @return entity_t
         */
            entity_t createEnnemiFrames(float x, float y, const std::shared_ptr<sf::Texture> &texture, float scale, short frame);
        /**
         * @brief Create a Blackband object
         *
         * @param rect
         * @param texture
         * @return entity_t
         */
            entity_t createBlackband(sf::IntRect rect, const std::shared_ptr<sf::Texture> &texture);
        /**
         * @brief Create a Score Coche object
         *
         * @param x
         * @param y
         * @param texture
         * @param scale
         * @return entity_t
         */
            entity_t createScoreCoche(float x, float y, const std::shared_ptr<sf::Texture> &texture, float scale);
        /**
         * @brief Create a Music object
         *
         * @param musicPath
         * @param volume
         * @param loop
         * @return entity_t
         */
            entity_t createMusic(const std::string &musicPath, float volume = 100.0f, bool loop = false);
        /**
         * @brief Create a Strobe object
         *
         * @param texture
         * @param x
         * @param y
         * @return entity_t
         */
            entity_t createStrobe(const std::shared_ptr<sf::Texture> &texture, float x, float y);
        /**
         * @brief Create a Text object
         *
         * @param text
         * @param font
         * @param x
         * @param y
         * @param size
         * @param color
         * @param style
         * @return entity_t
         */
            entity_t createText(const std::string &text, const std::shared_ptr<sf::Font> &font, const float &x = 0, const float &y = 0, const size_t &size = 20, const sf::Color &color = sf::Color::White, const sf::Text::Style &style = sf::Text::Style::Regular);
        /**
         * @brief Create a Loading Bar object
         *
         * @param x
         * @param y
         * @param texture
         * @param textureRect
         * @param scale
         * @return entity_t
         */
            entity_t createLoadingBar(float x, float y, const std::shared_ptr<sf::Texture> &texture, const std::shared_ptr<sf::Texture> &textureRect, float scale);
        /**
         * @brief Create a Sound object
         *
         * @param soundPath
         * @param volume
         * @param play
         * @return entity_t
         */
            entity_t createSound(const std::string &soundPath, float volume = 100.0f, bool play = false);
        /**
         * @brief Create a Bonus object
         *
         * @param texture
         * @param x
         * @param y
         * @param scale
         * @return entity_t
         */
            entity_t createBonus(const std::shared_ptr<sf::Texture> &texture, float x, float y, float scale);
        /**
         * @brief Create a Missile Annimated object
         *
         * @param x
         * @param y
         * @param texture
         * @param scale
         * @param frame
         * @return entity_t
         */
            entity_t createMissileAnnimated(float x, float y, const std::shared_ptr<sf::Texture> &texture, float scale, short frame);
        /**
         * @brief Create a Bomb object
         *
         * @param texture
         * @param x
         * @param y
         * @param scale
         * @return entity_t
         */
            entity_t createBomb(const std::shared_ptr<sf::Texture> &texture, float x, float y, float scale);
        /**
         * @brief Create a Pod object
         *
         * @param texture
         * @param scale
         * @return entity_t
         */
            entity_t createPod(const std::shared_ptr<sf::Texture> &texture, float scale);
        /**
         * @brief Create a Explosion object
         *
         * @param texture
         * @param x
         * @param y
         * @param scale
         * @return entity_t
         */
            entity_t createExplosion(const std::shared_ptr<sf::Texture> &texture, float x, float y, float scale);
        /**
         * @brief Create a Laser object
         *
         * @param texture
         * @param x
         * @param y
         * @param scale
         * @return entity_t
         */
            entity_t createLaser(const std::shared_ptr<sf::Texture> &texture, float x, float y, float scale);

            entity_t createParticle(sf::IntRect rect, sf::Color color, size_t duration);
            entity_t createParticle(sf::FloatRect rect, sf::Color color, size_t duration);
        private:
        /**
         * @brief Instance of the registry
         *
         */
            ECS::Registry &_registry;
    };
}