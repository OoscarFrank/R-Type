#pragma once

#include <cstddef>

namespace ECS {
    class Entity {
        public:
            explicit Entity(std::size_t id) : _id(id) {};
            ~Entity();

            operator std::size_t() const { return _id; }

        private:
            std::size_t _id;
    };
}
