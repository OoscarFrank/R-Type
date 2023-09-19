#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "../Core.hpp"
#include "../Error/MyError.hpp"
#include "./Components/Parallax.hpp"
#include "./Entity/Player.hpp"
#include "./Loader/Loader.hpp"

#define NOW std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

namespace game {
    class Game {
        public:
            Game();
            ~Game();
            int MainLoop();
        private:
            int CheckCloseWindow();
            sf::RenderWindow _window;
            sf::Event _event;
            long _lastTime;

            components::Parallax _parallax1;
            components::Parallax _parallax2;
            entity::Player _player1;
            loader::Loader _manager;
    };
}
