#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "../Core.hpp"
#include "../Error/MyError.hpp"
#include "Parallax.hpp"
#include "Player.hpp"
#define NOW std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

namespace client
{
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

            Parallax _parallax1;
            Parallax _parallax2;
            entity::Player _player1;
    };
}
