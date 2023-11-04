#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>
#include <chrono>


class Fbr
{
private:
    std::vector<sf::Uint8> _pixels;
    std::shared_ptr<sf::Texture> _texture;
    size_t _width;
    size_t _height;
    std::chrono::system_clock::time_point _lastFadeout;
public:
    Fbr(size_t width, size_t height);
    ~Fbr();
    void putPixel(size_t x, size_t y, sf::Color color);
    void drawLine(int x1, int y1, int x2, int y2, sf::Color color);
    void drawRect(int x, int y, size_t width, size_t height, sf::Color color);
    const std::shared_ptr<sf::Texture> &getTexture() { return _texture; };
    void refresh();
    void fadeout();
};
