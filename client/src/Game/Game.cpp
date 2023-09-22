#include "Game.hpp"

using namespace game;

Game::Game() :
    _manager(Loader())
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    this->_screenSize = {mode.width, mode.height};
    this->_window.create(sf::VideoMode(mode.width, mode.height), "R-TYPE");
    this->_lastTime = NOW;
}

Game::~Game()
{
}

int Game::MainLoop()
{
    while (this->_window.isOpen()) {
        long currentTime = NOW;
        float deltaTime = (currentTime - _lastTime) / 1000.0f;
        (void)deltaTime; // TO REMOVE WARNING
        _lastTime = currentTime;

        this->EventLoop(this->_window);
        // ALL SYSTEMS CALL
        this->_window.clear();
        // DRAW SYSTEM CALL
        this->_window.display();
    }
    return 0;
}
