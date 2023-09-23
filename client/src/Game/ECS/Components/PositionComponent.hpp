#pragma once

namespace ECS {
    namespace components {
        class PositionComponent {
            public:
                PositionComponent(): _x(0.0f), _y(0.0f) {}
                PositionComponent(float x, float y): _x(x), _y(y) {}

                void setX(float newX) { this->_x = newX; }
                void setY(float newY) { this->_y = newY;}

                float getX() const { return _x; }
                float getY() const { return _y; }

            private:
                float _x;
                float _y;
        };
    }
}
