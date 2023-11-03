#include "Pong.hpp"

#define FRAME_RATE_LIMIT 60
#define PADDLE_SPEED 10
#define BALL_SPEED 4

Pong::Pong():
    _window(sf::VideoMode(800, 600), "Pong"),
    _eventMemory(0),
    _score(0)
{
    _font.loadFromFile("assets/arial.ttf");

    _ecs.register_component<ECS::components::SpriteComponent>();
    _ecs.register_component<ECS::components::PositionComponent>();
    _ecs.register_component<ECS::components::TextureRectComponent>();
    _ecs.register_component<ECS::components::VelocityComponent>();
    _ecs.register_component<ECS::components::ControllableComponent>();
    _ecs.register_component<ECS::components::ParallaxComponent>();
    _ecs.register_component<ECS::components::MovableComponent>();
    _ecs.register_component<ECS::components::ScaleComponent>();
    _ecs.register_component<ECS::components::ButtonComponent>();
    _ecs.register_component<ECS::components::AnimationComponent>();
    _ecs.register_component<ECS::components::MusicComponent>();
    _ecs.register_component<ECS::components::TextComponent>();
    _ecs.register_component<ECS::components::RectangleShapeComponent>();
    _ecs.register_component<ECS::components::LoadingBarComponent>();
    _ecs.register_component<ECS::components::SoundComponent>();

    _playerPaddle = createPlayerPaddle(10, 10, 10, 100, sf::Color::Yellow);
    _autoPaddle = createAutoPaddle(780, 10, 10, 100, sf::Color::Yellow);
    _ball = createBall(400, 300, 10, sf::Color::Red);
    _scoreText = createText("0", _font, 400, 50, 30, sf::Color::White, sf::Text::Style::Bold);

    _window.setFramerateLimit(FRAME_RATE_LIMIT);
}

Pong::~Pong()
{
    _window.close();
}

void Pong::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.f / 60.f);

    while (_window.isOpen()) {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;

            handleEvents();
            handleAutoPaddle();
            handleBall();
            ECS::systems::PositionSystem().update(_ecs);
            ECS::systems::MovableSystem().update(_ecs, _pollEntityPositions);
            ECS::systems::ScaleSystem().update(_ecs);
            ECS::systems::TextSystem().update(_ecs, _pollTexts);
        }
        _pollEntityEvents.clear();
        ECS::systems::ControllableSystem().update(_ecs, _pollEntityEvents, _window, _eventMemory);
        _window.clear();
        ECS::systems::DrawSystem().update(_ecs, _window);
        _window.display();
    }
}

entity_t Pong::createPlayerPaddle(float x, float y, float width, float height, sf::Color color)
{
    entity_t paddle = _ecs.spawn_entity(100);
    _ecs.emplace_component<ECS::components::PositionComponent>(paddle, ECS::components::PositionComponent{x, y});
    _ecs.emplace_component<ECS::components::MovableComponent>(paddle, ECS::components::MovableComponent{});
    _ecs.emplace_component<ECS::components::RectangleShapeComponent>(paddle, ECS::components::RectangleShapeComponent{sf::Vector2f(width, height), color, sf::Vector2f(x, y)});
    _ecs.emplace_component<ECS::components::ControllableComponent>(paddle, ECS::components::ControllableComponent{sf::Keyboard::Key::Up, sf::Keyboard::Key::Down});
    return paddle;
}

entity_t Pong::createAutoPaddle(float x, float y, float width, float height, sf::Color color)
{
    entity_t paddle = _ecs.spawn_entity(100);
    _ecs.emplace_component<ECS::components::PositionComponent>(paddle, ECS::components::PositionComponent{x, y});
    _ecs.emplace_component<ECS::components::MovableComponent>(paddle, ECS::components::MovableComponent{});
    _ecs.emplace_component<ECS::components::RectangleShapeComponent>(paddle, ECS::components::RectangleShapeComponent{sf::Vector2f(width, height), color, sf::Vector2f(x, y)});
    return paddle;
}

entity_t Pong::createBall(float x, float y, float radius, sf::Color color)
{
    entity_t ball = _ecs.spawn_entity(99);
    _ecs.emplace_component<ECS::components::PositionComponent>(ball, ECS::components::PositionComponent{x, y});
    _ecs.emplace_component<ECS::components::VelocityComponent>(ball, ECS::components::VelocityComponent{-BALL_SPEED, -BALL_SPEED});
    _ecs.emplace_component<ECS::components::RectangleShapeComponent>(ball, ECS::components::RectangleShapeComponent{sf::Vector2f(radius, radius), color, sf::Vector2f(x, y)});
    return ball;
}

entity_t Pong::createText(const std::string &str, const sf::Font &font, float x, float y, size_t size, const sf::Color &color, const sf::Text::Style &style)
{
    entity_t text = _ecs.spawn_entity(200);
    _ecs.emplace_component<ECS::components::PositionComponent>(text, ECS::components::PositionComponent{x, y});
    _ecs.emplace_component<ECS::components::TextComponent>(text, ECS::components::TextComponent{font, str, size, color, style});
    return text;
}

void Pong::handleEvents()
{
    for (auto i = _pollEntityEvents.begin(); i != _pollEntityEvents.end(); i++) {
        if ((*i).getEvent() & DOWN) {
            ECS::components::PositionComponent pos = this->_ecs.getComponent<ECS::components::PositionComponent>(_playerPaddle);
            if (pos.getY() < _window.getSize().y - 100)
                _pollEntityPositions.push_back(ECS::systems::MovableSystem::EntityPos(_playerPaddle, pos.getX(), pos.getY() + PADDLE_SPEED));
        }
        if ((*i).getEvent() & UP) {
            ECS::components::PositionComponent pos = this->_ecs.getComponent<ECS::components::PositionComponent>(_playerPaddle);
            if (pos.getY() > 0)
                _pollEntityPositions.push_back(ECS::systems::MovableSystem::EntityPos(_playerPaddle, pos.getX(), pos.getY() - PADDLE_SPEED));
        }
    }
}

void Pong::handleAutoPaddle()
{
    ECS::components::PositionComponent ballPos = this->_ecs.getComponent<ECS::components::PositionComponent>(_ball);
    ECS::components::PositionComponent autoPaddlePos = this->_ecs.getComponent<ECS::components::PositionComponent>(_autoPaddle);
    ECS::components::RectangleShapeComponent autoPaddleRect = this->_ecs.getComponent<ECS::components::RectangleShapeComponent>(_autoPaddle);
    _pollEntityPositions.push_back(ECS::systems::MovableSystem::EntityPos(_autoPaddle, autoPaddlePos.getX(), ballPos.getY() - autoPaddleRect.getSize().y / 2));
}

void Pong::handleBall()
{
    ECS::components::PositionComponent ballPos = this->_ecs.getComponent<ECS::components::PositionComponent>(_ball);
    ECS::components::RectangleShapeComponent ballRect = this->_ecs.getComponent<ECS::components::RectangleShapeComponent>(_ball);
    ECS::components::PositionComponent playerPaddlePos = this->_ecs.getComponent<ECS::components::PositionComponent>(_playerPaddle);
    ECS::components::RectangleShapeComponent playerPaddleRect = this->_ecs.getComponent<ECS::components::RectangleShapeComponent>(_playerPaddle);
    ECS::components::PositionComponent autoPaddlePos = this->_ecs.getComponent<ECS::components::PositionComponent>(_autoPaddle);
    ECS::components::RectangleShapeComponent autoPaddleRect = this->_ecs.getComponent<ECS::components::RectangleShapeComponent>(_autoPaddle);

    if (ballPos.getX() < 0)
        _window.close();
    if (ballPos.getX() > _window.getSize().x)
        _window.close();
    if (ballPos.getY() <= 0)
        _ecs.modify_component<ECS::components::VelocityComponent>(_ball, [](ECS::components::VelocityComponent &vel) { vel.setDY(BALL_SPEED); });
    if (ballPos.getY() >= _window.getSize().y - ballRect.getSize().y)
        _ecs.modify_component<ECS::components::VelocityComponent>(_ball, [](ECS::components::VelocityComponent &vel) { vel.setDY(-BALL_SPEED); });
    if (ballPos.getX() < autoPaddlePos.getX() + autoPaddleRect.getSize().x && ballPos.getX() + ballRect.getSize().x > autoPaddlePos.getX() && ballPos.getY() < autoPaddlePos.getY() + autoPaddleRect.getSize().y && ballPos.getY() + ballRect.getSize().y > autoPaddlePos.getY())
        _ecs.modify_component<ECS::components::VelocityComponent>(_ball, [](ECS::components::VelocityComponent &vel) { vel.setDX(-BALL_SPEED); });
    if (ballPos.getX() < playerPaddlePos.getX() + playerPaddleRect.getSize().x && ballPos.getX() + ballRect.getSize().x > playerPaddlePos.getX() && ballPos.getY() < playerPaddlePos.getY() + playerPaddleRect.getSize().y && ballPos.getY() + ballRect.getSize().y > playerPaddlePos.getY()) {
        _score++;
        _ecs.modify_component<ECS::components::VelocityComponent>(_ball, [](ECS::components::VelocityComponent &vel) { vel.setDX(BALL_SPEED); });
        _pollTexts[_scoreText] = std::to_string(_score);
    }
}
