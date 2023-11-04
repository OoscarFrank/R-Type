#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../../Error/MyError.hpp"
#include <SFML/Audio.hpp>
#include <memory>

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
        /**
         * @brief Enum of things to load
         * 
         */
            enum toLoad {
                ParallaxFirstbkg,
                ParallaxSecondbkg,
                Player_move1, Player_move2, Player_move3, Player_move4,
                Missile,
                MissileRed,
                OrangeMissile,
                PurpleMissile,
                GreenMissile,
                FireBall,
                Explosion1,
                Monster1,
                Monster2,
                Monster3,
                Monster4,
                Boss1,
                Boss2,
                Boss3,
                Boss4,
                Boss5,
                Boss6,
                CreateRoomButton,
                JoinRoomButton,
                QuitButton,
                LeaveGame,
                MatchListButton,
                LooserScreen,
                MenuScreen,
                ChatBox,
                BlackPixel,
                ScoreCoche,
                RedPixel,
                GreenPixel,
                BluePixel,
                CyanPixel,
                YellowPixel,
                PurplePixel,
                WhitePixel,
                Arial,
                PressStart2P,
                playerLifeOutline,
                playerLifeContent,
                Bonus,
                Bomb,
                Pod1,
                Pod2,
                Pod3,
                Laser
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
         * @return const std::shared_ptr<sf::Texture>&
         */
            const std::shared_ptr<sf::Texture> &getTexture(toLoad type) const;
        /**
         * @brief Load a font from a path
         * 
         * @param path 
         * @param type 
         */
            void loadFont(const std::string path, toLoad type);
        /**
         * @brief Unload a font from a type
         * 
         * @param type 
         */
            void unloadFont(toLoad type);
        /**
         * @brief Get the Font object
         * 
         * @param type 
         * @return const std::shared_ptr<sf::Font>& 
         */
            const std::shared_ptr<sf::Font> &getFont(toLoad type) const;
        private:
        /**
         * @brief Store the textures
         * 
         */
            std::unordered_map<Loader::toLoad, std::shared_ptr<sf::Texture>> _textures;
        /**
         * @brief Store the fonts
         * 
         */
            std::unordered_map<Loader::toLoad, std::shared_ptr<sf::Font>> _fonts;
    };
}
