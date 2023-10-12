#pragma once

namespace ECS {
    namespace components {
        class ParallaxComponent {
            public:
            /**
             * @brief Construct a new Parallax Component object
             * 
             * @param scrollSpeed 
             * @param backgroundWidth 
             */
                ParallaxComponent(float scrollSpeed, float backgroundWidth): _scrollSpeed(scrollSpeed), _backgroundWidth(backgroundWidth) {}
            /**
             * @brief Get the Scroll Speed object
             * 
             * @return float 
             */
                float getScrollSpeed() const { return _scrollSpeed; }
            /**
             * @brief Get the Background Width object
             * 
             * @return float 
             */
                float getBackgroundWidth() const { return _backgroundWidth; }
            /**
             * @brief Set the Scroll Speed of the parallax
             * 
             * @param scrollSpeed 
             */
                void setScrollSpeed(float scrollSpeed) { _scrollSpeed = scrollSpeed; }

            private:
                float _scrollSpeed;
                float _backgroundWidth;
        };
    }
}
