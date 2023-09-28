#pragma once

namespace ECS {
    namespace components {
        class ParallaxComponent {
            public:
                ParallaxComponent(float scrollSpeed, float backgroundWidth): _scrollSpeed(scrollSpeed), _backgroundWidth(backgroundWidth) {}

                float getScrollSpeed() const { return _scrollSpeed; }
                float getBackgroundWidth() const { return _backgroundWidth; }

            private:
                float _scrollSpeed;
                float _backgroundWidth;
        };
    }
}
