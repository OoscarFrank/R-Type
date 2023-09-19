#include "Game.hpp"

using namespace game;
using namespace components;
using namespace entity;

Game::Game() :
    _parallax1({0, 0}, 120),
    _parallax2({0, 0}, 150),
    _player1({200, 200}, 100),
    _manager(loader::Loader())
{
    this->_window.create(sf::VideoMode(1920, 1080), "R-TYPE");
    this->_lastTime = NOW;
    this->_player1.setTexture(this->_manager.getTexture(loader::Loader::toLoad::Player));
    this->_parallax1.setTexture(this->_manager.getTexture(loader::Loader::toLoad::ParallaxFirstbkg));
    this->_parallax2.setTexture(this->_manager.getTexture(loader::Loader::toLoad::ParallaxSecondbkg));
}

Game::~Game()
{
}

int Game::MainLoop()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    this->_screenSize = {desktop.width, desktop.height};

    while (this->_window.isOpen()) {
        long currentTime = NOW;
        float deltaTime = (currentTime - _lastTime) / 1000.0f;
        _lastTime = currentTime;

        auto rocketCallback = [this]() {
            this->_rockets.push_back(entity::Rocket({this->_player1.getPosition().x + 100, this->_player1.getPosition().y + 90}, this->_manager));
        };
        this->EventLoop(this->_window, this->_player1, deltaTime, this->_screenSize, rocketCallback);
        this->_window.clear();
        this->_parallax1.update(deltaTime);
        this->_parallax1.draw(this->_window);
        this->_parallax2.update(deltaTime);
        this->_parallax2.draw(this->_window);

        this->_player1.update(deltaTime);
        this->_player1.draw(this->_window);

        for (auto &rocket: this->_rockets) {
            rocket.update(deltaTime);
            rocket.draw(this->_window);
            if (rocket.getPosition().x > this->_screenSize.x) {
                this->_rockets.erase(this->_rockets.begin());
            }
        }

        this->_window.display();
    }
    return 0;
}
