#include "EventManager.hpp"

using namespace game;
using namespace components;

EventManager::EventManager(): _timeElapsed(0)
{
}

EventManager::~EventManager()
{
}

void EventManager::EventLoop(sf::RenderWindow &window, game::entity::Player &player, float deltaTime, sf::Vector2u screenSize, std::function<void()> rocketCallback)
{
    while (window.pollEvent(this->_event)) {
        if (this->_event.type == sf::Event::Closed)
            window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (player.getPosition().x <= (screenSize.x - player._background.getGlobalBounds().width))
            player.setPosition({player.getPosition().x + (400.0f * deltaTime), player.getPosition().y});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if (player.getPosition().x > 0)
            player.setPosition({player.getPosition().x - (400.0f * deltaTime), player.getPosition().y});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        if (player.getPosition().y > 0)
            player.setPosition({player.getPosition().x, player.getPosition().y - (400.0f * deltaTime)});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if (player.getPosition().y <= (screenSize.y - player._background.getGlobalBounds().width))
            player.setPosition({player.getPosition().x, player.getPosition().y + (400.0f * deltaTime)});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (NOW - this->_timeElapsed > 250) {
            this->_timeElapsed = NOW;
            rocketCallback();
        }
    }
}

bool EventManager::isKeyPressed(keys key)
{
    return this->_event.type == (sf::Event::EventType) key;
}
