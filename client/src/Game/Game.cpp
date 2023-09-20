#include "Game.hpp"

using namespace game;
using namespace components;
using namespace entity;

Game::Game() :
    _parallax1({0, 0}, 120),
    _parallax2({0, 0}, 150),
    _player1({200, 200}, 100),
    _monster1({1500, 600}, 100),
    _manager(loader::Loader())
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    this->_screenSize = {mode.width, mode.height};
    this->_window.create(sf::VideoMode(mode.width, mode.height), "R-TYPE");
    this->_lastTime = NOW;
    this->_player1.setTexture(this->_manager.getTexture(loader::Loader::toLoad::Player));
    this->_monster1.setTexture(this->_manager.getTexture(loader::Loader::toLoad::Monster1));
    this->_parallax1.setTexture(this->_manager.getTexture(loader::Loader::toLoad::ParallaxFirstbkg));
    this->_parallax2.setTexture(this->_manager.getTexture(loader::Loader::toLoad::ParallaxSecondbkg));
}

Game::~Game()
{
}

int Game::MainLoop()
{
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

        for (auto it = this->_rockets.begin(); it != this->_rockets.end();++it) {
            it->update(deltaTime);
            it->draw(this->_window);
            if (it->getPosition().x > this->_screenSize.x) {
                this->_rockets.erase(it);
            }
        }

        this->_player1.update(deltaTime);
        this->_player1.draw(this->_window);

        this->_monster1.update(deltaTime);
        this->_monster1.draw(this->_window);


        this->_window.display();
    }
    return 0;
}
