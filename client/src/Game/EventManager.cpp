#include "EventManager.hpp"

using namespace game;

EventManager::EventManager(): _timeElapsed(0)
{
}

EventManager::~EventManager()
{
}

void EventManager::EventLoop(sf::RenderWindow &window, Network &_net)
{
    // while (window.pollEvent(this->_event)) {
    //     if (this->_event.type == sf::Event::Closed) {
    //         window.close();
    //         return;
    //     }
    // }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        // if (_net._ReaderThread.joinable())
        //     _net._ReaderThread.join();
        window.close();
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        _net.setInst(5);
        _net.send();
    }
}

bool EventManager::isKeyPressed(EventManager::keys key)
{
    return this->_event.type == (sf::Event::EventType) key;
}
