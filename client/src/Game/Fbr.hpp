#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>
#include <chrono>


class Fbr {
    private:
        std::vector<sf::Uint8> _pixels;
        std::shared_ptr<sf::Texture> _texture;
        size_t _width;
        size_t _height;
        std::chrono::system_clock::time_point _lastFadeout;
    public:
    /**
     * @brief Construct a new Fbr object
     * 
     * @param width 
     * @param height 
     */
        Fbr(size_t width, size_t height);
    /**
     * @brief Destroy the Fbr object
     * 
     */
        ~Fbr();
    /**
     * @brief Put a pixel in the buffer
     * 
     * @param x 
     * @param y 
     * @param color 
     */
        void putPixel(size_t x, size_t y, sf::Color color);
    /**
     * @brief Draw a line in the buffer
     * 
     * @param x1 
     * @param y1 
     * @param x2 
     * @param y2 
     * @param color 
     */
        void drawLine(int x1, int y1, int x2, int y2, sf::Color color);
    /**
     * @brief Draw a rectangle in the buffer
     * 
     * @param x 
     * @param y 
     * @param width 
     * @param height 
     * @param color 
     */
        void drawRect(int x, int y, size_t width, size_t height, sf::Color color);
    /**
     * @brief Get the Texture object
     * 
     * @return const std::shared_ptr<sf::Texture>& 
     */
        const std::shared_ptr<sf::Texture> &getTexture() { return _texture; };
    /**
     * @brief Refresh the buffer
     * 
     */
        void refresh();
    /**
     * @brief Fadeout the buffer
     * 
     */
        void fadeout();
};
