#pragma once

#include <iostream>

template <typename T>
class BoundingBox {
    public:
        BoundingBox(T x_, T y_, T w_, T h_);
        BoundingBox(const std::pair<T, T> &pos, const std::pair<T, T> &size);
        BoundingBox(const BoundingBox &other) = default;
        BoundingBox(BoundingBox &&other) = default;
        BoundingBox &operator=(const BoundingBox &other) = default;
        BoundingBox &operator=(BoundingBox &&other) = default;
        ~BoundingBox() = default;

        bool operator==(const BoundingBox &other) const;
        bool operator!=(const BoundingBox &other) const;

        bool collide(const BoundingBox &other) const;
        bool collide(const std::pair<T, T> &pos) const;
        bool collide(T x_, T y_) const;
        bool collide(const std::pair<T, T> &pos, const std::pair<T, T> &size) const;
        bool collide(T x_, T y_, T w_, T h_) const;

        T x;
        T y;
        T width;
        T height;
};

template <typename T>
BoundingBox<T>::BoundingBox(T x_, T y_, T w_, T h_):
    x(x_),
    y(y_),
    width(w_),
    height(h_)
{}

template <typename T>
BoundingBox<T>::BoundingBox(const std::pair<T, T> &pos, const std::pair<T, T> &size):
    x(pos.first),
    y(pos.second),
    width(size.first),
    height(size.second)
{}

template <typename T>
bool BoundingBox<T>::operator==(const BoundingBox &other) const
{
    return x == other.x && y == other.y && width == other.width && height == other.height;
}

template <typename T>
bool BoundingBox<T>::operator!=(const BoundingBox &other) const
{
    return !(*this == other);
}

template <typename T>
bool BoundingBox<T>::collide(const BoundingBox &other) const
{
    return collide(other.x, other.y, other.width, other.height);
}

template <typename T>
bool BoundingBox<T>::collide(const std::pair<T, T> &pos) const
{
    return collide(pos.first, pos.second);
}

template <typename T>
bool BoundingBox<T>::collide(T x_, T y_) const
{
    return collide(x_, y_, 0, 0);
}

template <typename T>
bool BoundingBox<T>::collide(const std::pair<T, T> &pos, const std::pair<T, T> &size) const
{
    return collide(pos.first, pos.second, size.first, size.second);
}

template <typename T>
bool BoundingBox<T>::collide(T x_, T y_, T w_, T h_) const
{
    return x < x_ + w_ && x + width > x_ && y < y_ + h_ && y + height > y_;
}
