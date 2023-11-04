#pragma once

namespace ECS {
    namespace components {
        class VelocityComponent {
        public:
        /**
         * @brief Construct a new Velocity Component object
         * 
         * @param dx 
         * @param dy 
         */
            VelocityComponent(float dx, float dy) : _initialDx(dx), _initialDy(dy), _dx(dx), _dy(dy) {}
        /**
         * @brief Get the Initial D X object
         * 
         * @return float 
         */
            float getInitialDX() const { return _initialDx; }
        /**
         * @brief Get the Initial D Y object
         * 
         * @return float 
         */
            float getInitialDY() const { return _initialDy; }
        /**
         * @brief Get the Current D Y object
         * 
         * @return float 
         */
            float getDX() const { return _dx; }
        /**
         * @brief Get the Current D X object
         * 
         * @return float 
         */
            float getDY() const { return _dy; }
        /**
         * @brief Set the Current D X object
         * 
         * @param dx 
         */
            void setDX(float dx) { _dx = dx; }
        /**
         * @brief Set the Current D Y object
         * 
         * @param dy 
         */
            void setDY(float dy) { _dy = dy; }

        private:
        /**
         * @brief Initial value of the velocity on the X axis
         * 
         */
            float _initialDx;
        /**
         * @brief Initial value of the velocity on the Y axis
         * 
         */
            float _initialDy;
        /**
         * @brief Value of the velocity on the X axis
         * 
         */
            float _dx;
        /**
         * @brief Value of the velocity on the Y axis
         * 
         */
            float _dy;
        };
    }
}
