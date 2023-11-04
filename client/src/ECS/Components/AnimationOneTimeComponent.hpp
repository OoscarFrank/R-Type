#pragma once

#include <iostream>

namespace ECS {
    namespace components {
        class AnimationOneTimeComponent {
            public:
            /**
            * @brief Construct a new AnimationOneTimeComponent Component object
            *
            */
                AnimationOneTimeComponent(){}
            /**
             * @brief Value of the current frame
             * 
             */
                size_t _frame = 0;
        };
    }
}
