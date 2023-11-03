#pragma once

#include <iostream>

namespace ECS {
    namespace components {
        class AnimationOneTimeComponent {
            public:
                AnimationOneTimeComponent(){}
                size_t _frame = 0;
        };
    }
}
