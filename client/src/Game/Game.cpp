#include "./Game.hpp"

using namespace client;

Game::Game() :
    _parallax1("./client/assets/parallax/background.png", {0, 0}, 120),
    _parallax2("./client/assets/parallax/background2.png", {0, 0}, 150),
    _player1("./client/assets/player.png", {200, 200}, 100)
{
    this->_window.create(sf::VideoMode(1920, 1080), "R-TYPE");
    this->_lastTime = NOW;
}

Game::~Game()
{
}

int Game::CheckCloseWindow()
{
    if (this->_event.type == sf::Event::Closed)
        return 1;
    if (this->_event.type == sf::Event::KeyPressed)
    {
        if (this->_event.key.code == sf::Keyboard::Escape)
            return 1;
    }
    return 0;
}
int Game::MainLoop()
{
    while (this->_window.isOpen()) {
        long currentTime = NOW;
        float deltaTime = (currentTime - _lastTime) / 1000.0f;
        _lastTime = currentTime;

        while (this->_window.pollEvent(this->_event))
            if (this->CheckCloseWindow() == 1)
                this->_window.close();

        this->_window.clear();
        this->_parallax1.update(deltaTime);
        this->_parallax1.draw(this->_window);
        this->_parallax2.update(deltaTime);
        this->_parallax2.draw(this->_window);

        this->_player1.draw(this->_window);

        this->_window.display();
    }
    return 0;
}
