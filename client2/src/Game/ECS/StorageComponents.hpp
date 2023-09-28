#pragma once

#include <vector>
#include <optional>
#include <stdexcept>

namespace ECS {
    template <typename Component>
    class StorageComponents {
        public:
            using value_type = std::optional<Component>;
            using reference_type = value_type &;
            using const_reference_type = value_type const &;
            using container_t = std::vector<value_type>;
            using size_type = typename container_t::size_type;
            using iterator = typename container_t::iterator;
            using const_iterator = typename container_t::const_iterator;

        public:
            StorageComponents() = default;
            StorageComponents(StorageComponents const &) = default;
            StorageComponents(StorageComponents &&) noexcept = default;
            ~StorageComponents() = default;
            StorageComponents &operator=(StorageComponents const &) = default;
            StorageComponents &operator=(StorageComponents &&) noexcept = default;

            reference_type operator[](size_t idx) {
                return _data[idx];
            }

            const_reference_type operator[](size_t idx) const {
                return _data[idx];
            }

            iterator begin() {
                return _data.begin();
            }

            const_iterator begin() const {
                return _data.begin();
            }

            const_iterator cbegin() const {
                return _data.cbegin();
            }

            iterator end() {
                return _data.end();
            }

            const_iterator end() const {
                return _data.end();
            }

            const_iterator cend() const {
                return _data.cend();
            }

            size_type size() const {
                return _data.size();
            }

            reference_type insert_at(size_type pos, Component const &component) {
                if (pos >= _data.size()) {
                    _data.resize(pos + 1);
                }
                _data[pos] = component;
                return _data[pos];
            }

            reference_type insert_at(size_type pos, Component &&component) {
                if (pos >= _data.size()) {
                    _data.resize(pos + 1);
                }
                _data[pos] = std::move(component);
                return _data[pos];
            }

            template <class... Params>
            reference_type emplace_at(size_type pos, Params &&...params) {
                if (pos >= _data.size()) {
                    _data.resize(pos + 1);
                }
                _data[pos].emplace(std::forward<Params>(params)...);
                return _data[pos];
            }

            void erase(size_type pos) {
                if (pos >= _data.size()) {
                    throw std::out_of_range("StorageComponents::erase");
                }
                _data[pos].reset();
            }

            size_type get_index(value_type const &component) const {
                for (size_type i = 0; i < _data.size(); ++i) {
                    if (_data[i] == component) {
                        return i;
                    }
                }
                return size_type(-1);
            }

        private:
            container_t _data;
    };
}