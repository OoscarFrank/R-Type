#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "../Entity/Player.hpp"

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
                void EventLoop(sf::RenderWindow &window, game::entity::Player &player);
                bool isKeyPressed(keys key);
            private:
                sf::Event _event;
        };
    }
}
