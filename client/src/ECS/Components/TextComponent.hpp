#pragma once

#include <SFML/Graphics.hpp>

namespace ECS {
    namespace components {
        class TextComponent {
            public:
            /**
             * @brief Construct a new Button Component object
             * 
             */
                TextComponent(const sf::Font &font, const std::string &str = "", const size_t &size = 20, const sf::Color &color = sf::Color::White, const sf::Text::Style &style = sf::Text::Style::Regular)
                {
                    _text.setFont(font);
                    _text.setString(str);
                    _text.setCharacterSize(size);
                    _text.setFillColor(color);
                    _text.setStyle(style);
                }
            /**
             * @brief Set the String object
             * 
             * @param str 
             */
                void setString(const std::string &str) {_text.setString(str); }
            /**
             * @brief Set the Size object
             * 
             * @param size 
             */
                void setSize(const size_t &size) { _text.setCharacterSize(size); }
            /**
             * @brief Set the Color object
             * 
             * @param color 
             */
                void setColor(const sf::Color &color) { _text.setFillColor(color); }
            /**
             * @brief Set the Style object
             * 
             * @param style 
             */
                void setStyle(const sf::Text::Style &style) { _text.setStyle(style); }
            /**
             * @brief Set the Position object
             * 
             * @param x 
             * @param y 
             */
                void setPosition(const float &x, const float &y) { _text.setPosition(sf::Vector2f(x, y)); }
            /**
             * @brief Set the Position object
             * 
             * @param pos 
             */
                void setPosition(const sf::Vector2f &pos) { _text.setPosition(pos); }
            /**
             * @brief Set the Scale object
             * 
             * @param x 
             * @param y 
             */
                void setScale(const float &x, const float &y)
                { 
                    if (_text.getScale().x != x || _text.getScale().y != y)
                        _text.setScale(sf::Vector2f(x, y));
                }
            /**
             * @brief Set the Scale object
             * 
             * @param scale 
             */
                void setScale(const sf::Vector2f &scale)
                {
                    if (_text.getScale() != scale)
                        _text.setScale(scale);
                }
            /**
             * @brief Get the Text object
             * 
             * @return const sf::Text& 
             */
                const sf::Text &getText() const { return _text; }
            private:
            /**
             * @brief Value of the text
             * 
             */
                sf::Text _text;
        };
    }
}
