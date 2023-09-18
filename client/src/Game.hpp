#pragma once

#include "Core.hpp"
#include "Utils/MyError.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

namespace Client
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
    };
}
