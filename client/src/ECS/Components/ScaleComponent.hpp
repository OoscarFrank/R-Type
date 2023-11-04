#pragma once

namespace ECS {
    namespace components {
        class ScaleComponent {
            public:
            /**
             * @brief Construct a new scale Component object
             *
             */
                ScaleComponent(): _scaleX(0.0f), _scaleY(0.0f) {}
            /**
             * @brief Construct a new scale Component object
             *
             * @param x
             * @param y
             */
                ScaleComponent(const float &scaleX, const float &scaleY): _scaleX(scaleX), _scaleY(scaleY) {}
            /**
             * @brief Set the scale object
             *
             * @param newScale
             */
                void setScale(const float &newScaleX, const float &newScaleY) {
                    this->_scaleX = newScaleX;
                    this->_scaleY = newScaleY;
                }
            /**
             * @brief Get the Scale X object
             *
             * @return float
             */
                float getScaleX() const { return _scaleX; }
            /**
             * @brief Get the Scale Y object
             * 
             * @return float 
             */
                float getScaleY() const { return _scaleY; }
            private:
            /**
             * @brief Value of the scale X
             * 
             */
                float _scaleX;
            /**
             * @brief Value of the scale Y
             * 
             */
                float _scaleY;
        };
    }
}
