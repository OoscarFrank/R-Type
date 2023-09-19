#include "EventManager.hpp"

using namespace game;
using namespace components;

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

void EventManager::EventLoop(sf::RenderWindow &window, game::entity::Player &player, float deltaTime, sf::Vector2u screenSize)
{
    while (window.pollEvent(this->_event)) {
        if (this->_event.type == sf::Event::Closed)
            window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (player.getPosition().x <= (screenSize.x))
            player.setPosition({player.getPosition().x + (400.0f * deltaTime), player.getPosition().y});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if (player.getPosition().x > 0)
            player.setPosition({player.getPosition().x - (400.0f * deltaTime), player.getPosition().y});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        player.setPosition({player.getPosition().x, player.getPosition().y - (400.0f * deltaTime)});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        player.setPosition({player.getPosition().x, player.getPosition().y + (400.0f * deltaTime)});
    }
}

bool EventManager::isKeyPressed(keys key)
{
    return this->_event.type == (sf::Event::EventType) key;
}
