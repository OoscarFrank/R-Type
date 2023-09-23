#pragma once

namespace ECS {
    namespace components {
        class VelocityComponent {
            public:
                VelocityComponent(float dx, float dy) : _dx(dx), _dy(dy) {}

                float getDX() const { return _dx; }
                float getDY() const { return _dy; }
                void setDX(float dx) { _dx = dx; }
                void setDY(float dy) { _dy = dy; }

            private:
                float _dx;
                float _dy;
        };
    }
}
