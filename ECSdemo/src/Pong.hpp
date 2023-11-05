#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "ECS.hpp"

class Pong {
    public:
        Pong();
        ~Pong();
        void run();

    private:
        entity_t createPlayerPaddle(float x, float y, float width, float height, sf::Color color);
        entity_t createAutoPaddle(float x, float y, float width, float height, sf::Color color);
        entity_t createBall(float x, float y, float radius, sf::Color color);
        entity_t createText(const std::string &str, const sf::Font &font, float x, float y, size_t size, const sf::Color &color, const sf::Text::Style &style);

        void handleEvents();
        void handleAutoPaddle();
        void handleBall();

        sf::RenderWindow _window;
        sf::Font _font;
        ECS::Registry _ecs;
        int _eventMemory;
        size_t _score;

        entity_t _playerPaddle;
        entity_t _autoPaddle;
        entity_t _ball;
        entity_t _scoreText;

        std::vector<ECS::systems::ControllableSystem::EntityEvent> _pollEntityEvents;
        std::vector<u_char> _pollKeys;
        std::vector<ECS::systems::MovableSystem::EntityPos> _pollEntityPositions;
        std::unordered_map<entity_t, std::string> _pollTexts;
};
