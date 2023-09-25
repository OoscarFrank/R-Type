#pragma once

namespace ECS {
    namespace components {
        class VelocityComponent {
        public:
            VelocityComponent(float dx, float dy) : _initialDx(dx), _initialDy(dy), _dx(dx), _dy(dy) {}

            float getInitialDX() const { return _initialDx; }
            float getInitialDY() const { return _initialDy; }
            float getDX() const { return _dx; }
            float getDY() const { return _dy; }
            void setDX(float dx) { _dx = dx; }
            void setDY(float dy) { _dy = dy; }

        private:
            float _initialDx;
            float _initialDy;
            float _dx;
            float _dy;
        };
    }
}
