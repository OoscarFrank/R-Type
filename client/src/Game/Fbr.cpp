#include "Fbr.hpp"

Fbr::Fbr(size_t width, size_t height)
{
    _pixels.reserve(width * height * 4);
    _texture = std::make_shared<sf::Texture>();
    _texture->create(width, height);
    _width = width;
    _height = height;
    this->drawRect(0, 0, width, height, sf::Color::Transparent);
}

Fbr::~Fbr()
{
}

void Fbr::putPixel(size_t x, size_t y, sf::Color color)
{  
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        _pixels[(y * _width + x) * 4 ] = color.r;
        _pixels[(y * _width + x) * 4  + 1] = color.g;
        _pixels[(y * _width + x) * 4  + 2] = color.b;
        _pixels[(y * _width + x) * 4  + 3] = color.a;
    }
}

void Fbr::drawLine(int x1, int y1, int x2, int y2, sf::Color color)
{
    for (float i = 0; i < 1; i += 0.0001) {
        int px = x1 + (x2 - x1) * i;
        int py = y1 + (y2 - y1) * i;
        this->putPixel(px, py, color);
    }
}

void Fbr::drawRect(int x, int y, size_t width, size_t height, sf::Color color)
{
    for (size_t i = x; i < width + x; i++)
    {
        for (size_t j = y; j < height + y; j++)
        {
            putPixel(i, j, color);
        }
    }
}

void Fbr::fadeout()
{
    if (std::chrono::system_clock::now() - this->_lastFadeout >= std::chrono::milliseconds(10)) {
        for (size_t i = 0; i < _width * _height * 4; i += 4) {
            if (_pixels[i + 3] > 0) {
                if (_pixels[i + 3] >= 10)
                    _pixels[i + 3] -= 10;
                else
                    _pixels[i + 3] = 0;
            }
        }
        _lastFadeout = std::chrono::system_clock::now();
    }
}

void Fbr::refresh()
{
    _texture->update(_pixels.data(), _width, _height, 0, 0);
}
