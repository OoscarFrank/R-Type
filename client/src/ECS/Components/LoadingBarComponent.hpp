#pragma once

namespace ECS {
    namespace components {
        class LoadingBarComponent {
            public:
            /**
             * @brief Construct a new Loading Bar Component object
             * 
             * @param maximumWidth 
             * @param textureWidth 
             */
                LoadingBarComponent(float maximumWidth, float textureWidth): _maximumWidth(maximumWidth), _textureWidth(textureWidth) {}
            /**
             * @brief Get the Maximum Width object
             * 
             * @return float 
             */
                float getMaximumWidth() const { return _maximumWidth; }
            /**
             * @brief Get the Texture Width object
             * 
             * @return float 
             */
                float getTextureWidth() const { return _textureWidth; }
            /**
             * @brief Set the Maximum Width object
             * 
             * @param maximumWidth 
             */
                void setMaximumWidth(float maximumWidth) { _maximumWidth = maximumWidth; }
            /**
             * @brief Set the Texture Width object
             * 
             * @param textureWidth 
             */
                void setTextureWidth(float textureWidth) { _textureWidth = textureWidth; }
            /**
             * @brief Calculate the new width of the loading bar
             * 
             * @param currentWidth 
             * @return float 
             */
                float calculate(float currentWidth) {
                    float newWidth = (currentWidth / _maximumWidth) * _textureWidth;
                    if (newWidth < 0.0f)
                        newWidth = 0.0f;
                    return newWidth;
                }

            private:
            /**
             * @brief Value of the maximum width
             * 
             */
                float _maximumWidth;
            /**
             * @brief Value of the texture width
             * 
             */
                float _textureWidth;
        };
    }
}
