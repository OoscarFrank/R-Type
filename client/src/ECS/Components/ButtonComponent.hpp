#pragma once
#include <functional>

namespace ECS {
    namespace components {
        class ButtonComponent {
            public:
                /**
                 * @brief Construct a new Button Component object
                 *
                 * @param callback
                 */
                ButtonComponent(std::function<void()> callback) {
                    this->_callback = callback;
                }


                /**
                 * @brief Call the callback function
                 *
                 */
                void executeAction() {
                    if (this->_callback) {
                        _callback();
                    }
                }

            private:
                std::function<void()> _callback;
        };
    }
}
