#include "EventManager.hpp"

using namespace game;

EventManager::EventManager(): _timeElapsed(0)
{
}

EventManager::~EventManager()
{
}

void EventManager::EventLoop(sf::RenderWindow &window)
{
    while (window.pollEvent(this->_event)) {
        if (this->_event.type == sf::Event::Closed) {
            window.close();
            return;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
        return;
    }
}

bool EventManager::isKeyPressed(EventManager::keys key)
{
    return this->_event.type == (sf::Event::EventType) key;
}
