#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <typeindex>
#include <any>
#include <functional>
#include "StorageComponents.hpp"
#include <iostream>
#include "./Components/Components.hpp"

using entity_t = std::size_t;

namespace ECS
{
    class Registry
    {
    public:
        /**
         * @brief Add a component object to the registry
         *
         * @tparam Component
         * @return StorageComponents<Component>&
         */
        template <class Component>
        StorageComponents<Component> &register_component()
        {
            auto typeId = std::type_index(typeid(Component));
            auto &array = _components_arrays[typeId];
            if (!array.has_value())
                array = StorageComponents<Component>();
            return std::any_cast<StorageComponents<Component> &>(array);
        }

        /**
         * @brief List components by type
         *
         * @tparam Component
         * @return StorageComponents<Component>&
         */
        template <class Component>
        StorageComponents<Component> &get_components()
        {
            auto typeId = std::type_index(typeid(Component));
            auto &array = _components_arrays[typeId];
            if (!array.has_value())
                throw std::runtime_error("Component array not registered.");
            return std::any_cast<StorageComponents<Component> &>(array);
        }

        /**
         * @brief List components by type
         *
         * @tparam Component
         * @return StorageComponents<Component> const&
         */
        template <class Component>
        StorageComponents<Component> const &get_components() const
        {
            auto typeId = std::type_index(typeid(Component));
            auto it = _components_arrays.find(typeId);
            if (it == _components_arrays.end() || !it->second.has_value())
                throw std::runtime_error("Component array not registered.");
            return std::any_cast<const StorageComponents<Component> &>(it->second);
        }

        /**
         * @brief Check if the entity has the component
         *
         * @tparam Component
         * @param entity
         * @return true
         * @return false
         */
        template <typename Component>
        bool hasComponent(entity_t const &entity)
        {
            if (_entity_to_index.find(entity) == _entity_to_index.end())
            {
                throw std::runtime_error("Entity not found.");
            }
            auto &comp_array = get_components<Component>();
            size_t index = entity;
            if (index >= comp_array.size())
            {
                return false;
            }
            return comp_array[index].has_value();
        }
        /**
         * @brief Get a component object of the entity
         *
         * @tparam Component
         * @param entity
         * @return Component&
         */
        template <typename Component>
        Component &getComponent(entity_t const &entity)
        {
            if (_entity_to_index.find(entity) == _entity_to_index.end())
            {
                throw std::runtime_error("Entity not found.");
            }

            auto &comp_array = get_components<Component>();
            size_t index = entity;

            if (index >= comp_array.size())
            {
                throw std::runtime_error("Entity does not have the specified component.");
            }

            auto &optionalComponent = comp_array[index];

            if (!optionalComponent.has_value())
            {
                throw std::runtime_error("Entity does not have the specified component.");
            }
            return optionalComponent.value();
        }

        /**
         * @brief Create new entity
         *
         * @return entity_t
         */
        entity_t spawn_entity(std::size_t zIndex = 0, bool state = true)
        {
            entity_t entity = _next_entity_id++;
            _entity_to_index[entity] = std::make_pair(state, zIndex);

            _entities.push_back(entity);
            return entity;
        }

        void disableEntity(entity_t const &entity)
        {
            if (_entity_to_index.find(entity) == _entity_to_index.end()) {
                throw std::runtime_error("Entity not found.");
            }
            _entity_to_index[entity].first = false;
        }

        void enableEntity(entity_t const &entity)
        {
            if (_entity_to_index.find(entity) == _entity_to_index.end()) {
                throw std::runtime_error("Entity not found.");
            }
            _entity_to_index[entity].first = true;
        }

        bool isEntityEnabled(entity_t const &entity)
        {
            if (_entity_to_index.find(entity) == _entity_to_index.end()) {
                throw std::runtime_error("Entity not found.");
            }
            return _entity_to_index[entity].first;
        }

        /**
         * @brief Get an entity from an index
         *
         * @param idx
         * @return entity_t
         */
        entity_t entity_from_index(std::size_t idx)
        {
            if (idx >= _next_entity_id)
                throw std::runtime_error("Entity index out of range.");
            return _entities[idx];
        }

        bool isEntityExist(entity_t entity)
        {
            return _entity_to_index.find(entity) != _entity_to_index.end();
        }
        /**
         * @brief Destroy an entity
         *
         * @tparam Component
         * @param e
         */
        void kill_entity(entity_t const &e)
        {
            auto it = _entity_to_index.find(e);
            if (it == _entity_to_index.end()) {
                throw std::runtime_error("Entity not found.");
            }

            if (this->hasComponent<components::ControllableComponent>(e))
                this->remove_component<components::ControllableComponent>(e);

            if (this->hasComponent<components::MovableComponent>(e))
                this->remove_component<components::MovableComponent>(e);

            if (this->hasComponent<components::ParallaxComponent>(e))
                this->remove_component<components::ParallaxComponent>(e);

            if (this->hasComponent<components::PositionComponent>(e))
                this->remove_component<components::PositionComponent>(e);

            if (this->hasComponent<components::SpriteComponent>(e))
                this->remove_component<components::SpriteComponent>(e);

            if (this->hasComponent<components::TextureRectComponent>(e))
                this->remove_component<components::TextureRectComponent>(e);

            if (this->hasComponent<components::VelocityComponent>(e))
                this->remove_component<components::VelocityComponent>(e);

            if (this->hasComponent<components::AnimationComponent>(e))
                this->remove_component<components::AnimationComponent>(e);

            if (this->hasComponent<components::ButtonComponent>(e))
                this->remove_component<components::ButtonComponent>(e);

            if (this->hasComponent<components::ScaleComponent>(e))
                this->remove_component<components::ScaleComponent>(e);

            if (this->hasComponent<components::TextComponent>(e))
                this->remove_component<components::TextComponent>(e);

            if (this->hasComponent<components::MusicComponent>(e))
                this->remove_component<components::MusicComponent>(e);

            if (this->hasComponent<components::SoundComponent>(e))
                this->remove_component<components::SoundComponent>(e);

            if (this->hasComponent<components::LoadingBarComponent>(e))
                this->remove_component<components::LoadingBarComponent>(e);

            
            if (this->hasComponent<components::AnimationOneTimeComponent>(e))
                this->remove_component<components::AnimationOneTimeComponent>(e);

            _entity_to_index.erase(it);
            if (_entity_sprite_order.count(e) > 0) {
                auto range = _entity_sprite_order.equal_range(e);
                for (auto it = range.first; it != range.second; ++it) {
                    if (it->second == e) {
                        _entity_sprite_order.erase(it);
                        break;
                    }
                }
            }
        }
        /**
         * @brief Add a construct component in the registry
         *
         * @tparam Component
         * @param to
         * @param component
         * @return StorageComponents<Component>::reference_type
         */
        template <typename Component>
        typename StorageComponents<Component>::reference_type add_component(entity_t const &to, Component &&component)
        {
            if (_entity_to_index.find(to) == _entity_to_index.end())
                throw std::runtime_error("Entity not found.");
            auto &comp_array = get_components<Component>();
            return comp_array.insert_at(to, std::forward<Component>(component));
        }
        /**
         * @brief Copy a component in the registry
         *
         * @tparam Component
         * @tparam Params
         * @param to
         * @param params
         * @return StorageComponents<Component>::reference_type
         */
        template <typename Component, typename... Params>
        typename StorageComponents<Component>::reference_type emplace_component(entity_t const &to, Params &&...params)
        {
            if (_entity_to_index.find(to) == _entity_to_index.end())
                throw std::runtime_error("Entity not found.");
            auto &comp_array = get_components<Component>();
            if (std::is_same<Component, components::SpriteComponent>::value) {
                auto entityIndexIt = _entity_to_index.find(to);
                if (entityIndexIt != _entity_to_index.end()) {
                    size_t entityIndex = entityIndexIt->second.second;
                    _entity_sprite_order.insert(std::make_pair(entityIndex, to));
                }
            }
            return comp_array.emplace_at(to, std::forward<Params>(params)...);
        }
        /**
         * @brief Modify a component in the registry
         *
         * @tparam Component
         * @param entity
         * @param callback
         */
        template <typename Component>
        void modify_component(entity_t const &entity, std::function<void(Component&)> callback)
        {
            if (_entity_to_index.find(entity) == _entity_to_index.end())
                throw std::runtime_error("Entity not found.");

            auto &comp_array = get_components<Component>();
            size_t index = entity;
            if (index >= comp_array.size())
                throw std::runtime_error("Entity does not have the specified component.");

            auto &optionalComponent = comp_array[index];
            if (!optionalComponent.has_value())
                throw std::runtime_error("Entity does not have the specified component.");

            callback(optionalComponent.value());
        }
        /**
         * @brief Remove a component from the registry
         *
         * @tparam Component
         * @param from
         */
        template <typename Component>
        void remove_component(entity_t const &from)
        {
            if (_entity_to_index.find(from) == _entity_to_index.end())
                throw std::runtime_error("Entity not found.");
            auto &comp_array = get_components<Component>();
            comp_array.erase(from);
        }

        std::multimap<size_t, entity_t> const &get_entity_sprite_order() const {
            return _entity_sprite_order;
        }

    private:
        std::multimap<size_t, entity_t> _entity_sprite_order;
        std::unordered_map<std::type_index, std::any> _components_arrays;
        std::map<entity_t, std::pair<bool, size_t>> _entity_to_index;
        entity_t _next_entity_id = 0;
        std::vector<entity_t> _entities;
    };
}
