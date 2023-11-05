#pragma once

#include <cstddef>

namespace ECS {
    class Entity {
        public:
        /**
         * @brief Construct a new Entity object
         * 
         * @param id 
         */
            explicit Entity(std::size_t id) : _id(id) {};
        /**
         * @brief Destroy the Entity object
         * 
         */
            ~Entity();
        /**
         * @brief Operator std::size_t
         * 
         * @return std::size_t 
         */
            operator std::size_t() const { return _id; }

        private:
        /**
         * @brief ID of the entity
         * 
         */
            std::size_t _id;
    };
}
