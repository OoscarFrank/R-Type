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
        if (this->_event.type == sf::Event::KeyPressed) {
            if (this->_event.key.code == sf::Keyboard::Escape) window.close();
            if (this->_event.key.code == sf::Keyboard::Up) {
                player.setPosition({900, 900});
            }
        }
    }
}

bool EventManager::isKeyPressed(keys key)
{
    return this->_event.type == (sf::Event::EventType) key;
}
