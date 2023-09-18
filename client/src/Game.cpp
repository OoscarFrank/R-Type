#include "./Game.hpp"

using namespace Client;

Game::Game()
{
    this->_window.create(sf::VideoMode(1920, 1080), "R-TYPE");
}

Game::~Game()
{
}

int Game::CheckCloseWindow()
{
    if (this->_event.type == sf::Event::Closed)
        return 1;
    if (this->_event.type == sf::Event::KeyPressed) {
        if (this->_event.key.code == sf::Keyboard::Escape)
            return 1;
    }
    return 0;
}

int Game::MainLoop()
{
    while (this->_window.isOpen()) {
        while (this->_window.pollEvent(this->_event))
            if (this->CheckCloseWindow() == 1) this->_window.close();

        this->_window.clear();
        this->_window.display();
    }

    return 0;
}
