#include "EventManager.hpp"

using namespace game;
using namespace components;

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

void EventManager::EventLoop(sf::RenderWindow &window, game::entity::Player &player)
{
    while (window.pollEvent(this->_event)) {
        if (this->_event.type == sf::Event::Closed)
            window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player.setPosition({player.getPosition().x + 0.4f, player.getPosition().y});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player.setPosition({player.getPosition().x - 0.4f, player.getPosition().y});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        player.setPosition({player.getPosition().x, player.getPosition().y - 0.4f});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        player.setPosition({player.getPosition().x, player.getPosition().y + 0.4f});
    }
}

bool EventManager::isKeyPressed(keys key)
{
    return this->_event.type == (sf::Event::EventType) key;
}
