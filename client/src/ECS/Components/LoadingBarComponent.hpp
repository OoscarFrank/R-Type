#pragma once

namespace ECS {
    namespace components {
        class LoadingBarComponent {
            public:
                LoadingBarComponent(float maximumWidth, float textureWidth): _maximumWidth(maximumWidth), _textureWidth(textureWidth) {}
                float getMaximumWidth() const { return _maximumWidth; }
                float getTextureWidth() const { return _textureWidth; }
                void setMaximumWidth(float maximumWidth) { _maximumWidth = maximumWidth; }
                void setTextureWidth(float textureWidth) { _textureWidth = textureWidth; }

                float calculate(float currentWidth) {
                    float newWidth = (currentWidth / _maximumWidth) * _textureWidth;
                    if (newWidth < 0.0f)
                        newWidth = 0.0f;
                    return newWidth;
                }

            private:
                float _maximumWidth;
                float _textureWidth;
        };
    }
}
