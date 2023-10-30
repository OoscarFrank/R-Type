#pragma once

namespace ECS {
    namespace components {
        class PositionComponent {
            public:
            /**
             * @brief Construct a new Position Component object
             * 
             */
                PositionComponent(): _x(0.0f), _y(0.0f) {}
            /**
             * @brief Construct a new Position Component object
             * 
             * @param x 
             * @param y 
             */
                PositionComponent(float x, float y): _x(x), _y(y) {}
            /**
             * @brief Set the X object
             * 
             * @param newX 
             */
                void setX(float newX) { this->_x = newX; }
            /**
             * @brief Set the Y object
             * 
             * @param newY 
             */
                void setY(float newY) { this->_y = newY;}
            /**
             * @brief Get the X object
             * 
             * @return float 
             */
                float getX() const { return _x; }
            /**
             * @brief Get the Y object
             * 
             * @return float 
             */
                float getY() const { return _y; }

            private:
                float _x;
                float _y;
        };
    }
}
