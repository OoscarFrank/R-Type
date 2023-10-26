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

                void setString(const std::string &str) {_text.setString(str); }
                void setSize(const size_t &size) { _text.setCharacterSize(size); }
                void setColor(const sf::Color &color) { _text.setFillColor(color); }
                void setStyle(const sf::Text::Style &style) { _text.setStyle(style); }
                void setPosition(const float &x, const float &y) { _text.setPosition(sf::Vector2f(x, y)); }
                void setPosition(const sf::Vector2f &pos) { _text.setPosition(pos); }
                void setScale(const float &x, const float &y)
                { 
                    if (_text.getScale().x != x || _text.getScale().y != y)
                        _text.setScale(sf::Vector2f(x, y));
                }

                void setScale(const sf::Vector2f &scale)
                {
                    if (_text.getScale() != scale)
                        _text.setScale(scale);
                }

                const sf::Text &getText() const { return _text; }
            
            private:
                sf::Text _text;
        };
    }
}
