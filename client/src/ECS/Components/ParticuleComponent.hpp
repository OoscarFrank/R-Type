#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>

namespace ECS {
    namespace components {
        class ParticuleComponent {
            public:
            /**
             * @brief Construct a new Particule Component object
             *
             * @param rect
             * @param color
             * @param duration
             */
                ParticuleComponent(const sf::IntRect &rect, const sf::Color &color, const size_t &duration): _rect(rect), _color(color), _duration(duration) {
                    _time = std::chrono::system_clock::now();
                }
            /**
             * @brief Get the Time object
             *
             * @return std::chrono::system_clock::time_point
             */
                std::chrono::system_clock::time_point getTime() const {
                    return _time;
                }
            /**
             * @brief Get the Rect object
             *
             * @return sf::IntRect
             */
                sf::IntRect getRect() const {
                    return _rect;
                }
            /**
             * @brief Get the Color object
             *
             * @return sf::Color
             */
                sf::Color getColor() const {
                    return _color;
                }
            /**
             * @brief Get the Duration object
             *
             * @return size_t
             */
                size_t getDuration() const {
                    return _duration;
                }

            private:
            /**
             * @brief Time of the particule
             *
             */
                std::chrono::system_clock::time_point _time;
            /**
             * @brief Rect of the particule
             *
             */
                sf::IntRect _rect;
            /**
             * @brief Color of the particule
             *
             */
                sf::Color _color;
            /**
             * @brief Duration of the particule
             *
             */
                size_t _duration;
        };
    }
}
