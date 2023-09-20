#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "../Entity/Player.hpp"
#include <functional>

#define NOW std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

namespace game {
    namespace components {
        class EventManager {
            public:
                EventManager();
                ~EventManager();
                enum keys {
                    Up,
                    Down,
                    Left,
                    Right,
                    Space,
                    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
                    Escape
                };
                void EventLoop(sf::RenderWindow &window, game::entity::Player &player, float deltaTime, sf::Vector2u screenSize, std::function<void()> rocketCallback);
                bool isKeyPressed(keys key);
            private:
                sf::Event _event;
                long long _timeElapsed;
        };
    }
}
