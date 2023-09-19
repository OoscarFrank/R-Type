#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "../Core.hpp"
#include "../Error/MyError.hpp"
#include "Parallax.hpp"

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
            Parallax _parallax1;
            Parallax _parallax2;
    };
}
