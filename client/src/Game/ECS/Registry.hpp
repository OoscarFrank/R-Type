#pragma once

#include <map>
#include <unordered_map>
#include <typeindex>
#include <any>
#include "StorageComponents.hpp"

namespace ECS {
    class registry {
        public:
            template<class Component>
            StorageComponents<Component> &register_component() {
                auto typeId = std::type_index(typeid(Component));
                auto &array = _components_arrays[typeId];
                if (!array.has_value())
                    array = StorageComponents<Component>();
                return std::any_cast<StorageComponents<Component> &>(array);
            }

            template <class Component>
            StorageComponents<Component> &get_components() {
                auto typeId = std::type_index(typeid(Component));
                auto &array = _components_arrays[typeId];
                if (!array.has_value())
                    throw std::runtime_error("Component array not registered.");
                return std::any_cast<StorageComponents<Component> &>(array);
            }

            template <class Component>
            StorageComponents<Component> const &get_components() const {
                auto typeId = std::type_index(typeid(Component));
                auto it = _components_arrays.find(typeId);
                if (it == _components_arrays.end() || !it->second.has_value())
                    throw std::runtime_error("Component array not registered.");
                return std::any_cast<const StorageComponents<Component> &>(it->second);
            }

        public:
            using entity_t = std::size_t;

            entity_t spawn_entity() {
                entity_t entity = _next_entity_id++;
                _entity_to_index[entity] = true;
                _entities.push_back(entity);
                return entity;
            }

            entity_t entity_from_index(std::size_t idx) {
                if (idx >= _next_entity_id)
                    throw std::runtime_error("Entity index out of range.");
                return _entities[idx];
            }

            template <typename Component>
            void kill_entity(entity_t const &e) {
                if (_entity_to_index.find(e) == _entity_to_index.end())
                    throw std::runtime_error("Entity not found.");
                _entity_to_index[e] = false;
                for (auto &array : _components_arrays) {
                    if (array.second.has_value()) {
                        auto &comp_array = std::any_cast<StorageComponents<Component> &>(array.second);
                        comp_array.erase(e);
                    }
                }
            }

            template <typename Component>
            typename StorageComponents<Component>::reference_type add_component(entity_t const &to, Component &&component) {
                if (_entity_to_index.find(to) == _entity_to_index.end())
                    throw std::runtime_error("Entity not found.");
                auto &comp_array = get_components<Component>();
                return comp_array.insert_at(to, std::forward<Component>(component));
            }

            template <typename Component, typename...Params>
            typename StorageComponents<Component>::reference_type emplace_component(entity_t const &to, Params &&...params) {
                if (_entity_to_index.find(to) == _entity_to_index.end())
                    throw std::runtime_error("Entity not found.");
                auto &comp_array = get_components<Component>();
                return comp_array.emplace_at(to, std::forward<Params>(params)...);
            }

            template <typename Component>
            void remove_component(entity_t const &from) {
                if (_entity_to_index.find(from) == _entity_to_index.end())
                    throw std::runtime_error("Entity not found.");
                auto &comp_array = get_components<Component>();
                comp_array.erase(from);
            }

        private:
            std::unordered_map<std::type_index, std::any> _components_arrays;
            std::map<entity_t, bool> _entity_to_index;
            entity_t _next_entity_id = 0;
            std::vector<entity_t> _entities;
    };
}
