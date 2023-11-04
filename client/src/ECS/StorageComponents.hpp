#pragma once

#include <vector>
#include <optional>
#include <stdexcept>

namespace ECS {
    template <typename Component>
    class StorageComponents {
        public:
        /**
         * @brief Using of the value type
         * 
         */
            using value_type = std::optional<Component>;
            using reference_type = value_type &;
            using const_reference_type = value_type const &;
            using container_t = std::vector<value_type>;
            using size_type = typename container_t::size_type;
            using iterator = typename container_t::iterator;
            using const_iterator = typename container_t::const_iterator;

        public:
        /**
         * @brief Construct a new Storage Components object
         * 
         */
            StorageComponents() = default;
        /**
         * @brief Construct a new Storage Components object
         * 
         */
            StorageComponents(StorageComponents const &) = default;
        /**
         * @brief Construct a new Storage Components object
         * 
         */
            StorageComponents(StorageComponents &&) noexcept = default;
        /**
         * @brief Destroy the Storage Components object
         * 
         */
            ~StorageComponents() = default;
            StorageComponents &operator=(StorageComponents const &) = default;
            StorageComponents &operator=(StorageComponents &&) noexcept = default;
        /**
         * @brief Overload of the [] operator
         * 
         * @param idx 
         * @return reference_type 
         */
            reference_type operator[](size_t idx) {
                return _data[idx];
            }
        /**
         * @brief Overload of the [] operator
         * 
         * @param idx 
         * @return const_reference_type 
         */
            const_reference_type operator[](size_t idx) const {
                return _data[idx];
            }
        /**
         * @brief Get the begin iterator
         * 
         * @return iterator 
         */
            iterator begin() {
                return _data.begin();
            }
        /**
         * @brief Get the begin iterator
         * 
         * @return const_iterator 
         */
            const_iterator begin() const {
                return _data.begin();
            }
        /**
         * @brief Get the cbegin iterator
         * 
         * @return const_iterator 
         */
            const_iterator cbegin() const {
                return _data.cbegin();
            }
        /**
         * @brief Get the end iterator
         * 
         * @return iterator 
         */
            iterator end() {
                return _data.end();
            }
        /**
         * @brief Get the end iterator
         * 
         * @return const_iterator 
         */
            const_iterator end() const {
                return _data.end();
            }
        /**
         * @brief Get the cend iterator
         * 
         * @return const_iterator 
         */
            const_iterator cend() const {
                return _data.cend();
            }
        /**
         * @brief Get the size of the storage
         * 
         * @return size_type 
         */
            size_type size() const {
                return _data.size();
            }
        /**
         * @brief Insert a component of the storage
         * 
         * @param pos 
         * @param component 
         * @return reference_type 
         */
            reference_type insert_at(size_type pos, Component const &component) {
                if (pos >= _data.size()) {
                    _data.resize(pos + 1);
                }
                _data[pos] = component;
                return _data[pos];
            }
        /**
         * @brief Insert a component of the storage
         * 
         * @param pos 
         * @param component 
         * @return reference_type 
         */
            reference_type insert_at(size_type pos, Component &&component) {
                if (pos >= _data.size()) {
                    _data.resize(pos + 1);
                }
                _data[pos] = std::move(component);
                return _data[pos];
            }
        /**
         * @brief Emplace a component of the storage
         * 
         * @tparam Params 
         * @param pos 
         * @param params 
         * @return reference_type 
         */
            template <class... Params>
            reference_type emplace_at(size_type pos, Params &&...params) {
                if (pos >= _data.size()) {
                    _data.resize(pos + 1);
                }
                _data[pos].emplace(std::forward<Params>(params)...);
                return _data[pos];
            }
        /**
         * @brief Erase a component of the storage
         * 
         * @param pos 
         */
            void erase(size_type pos) {
                if (pos >= _data.size()) {
                    throw std::out_of_range("StorageComponents::erase");
                }
                _data[pos].reset();
            }
        /**
         * @brief Get the index object
         * 
         * @param component 
         * @return size_type 
         */
            size_type get_index(value_type const &component) const {
                for (size_type i = 0; i < _data.size(); ++i) {
                    if (_data[i] == component) {
                        return i;
                    }
                }
                return size_type(-1);
            }

        private:
        /**
         * @brief Container of the storage
         * 
         */
            container_t _data;
    };
}