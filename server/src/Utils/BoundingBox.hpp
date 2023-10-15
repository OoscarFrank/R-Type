#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

#include <iostream>

/**
 * @brief A bounding box is a rectangle used to check collision
 *
 * @tparam T The type of the position and size of the box (int, float, etc.)
 */
template <typename T>
class BoundingBox {
    public:
        /**
         * @brief Construct a new Bounding Box object
         *
         * @param x_ The x position of the bounding box
         * @param y_ The y position of the bounding box
         * @param w_ The width of the bounding box
         * @param h_ The height of the bounding box
         */
        BoundingBox(T x_, T y_, T w_, T h_);
        /**
         * @brief Construct a new Bounding Box object
         *
         * @param pos The position of the bounding box (x, y)
         * @param size The size of the bounding box (width, height)
         */
        BoundingBox(const std::pair<T, T> &pos, const std::pair<T, T> &size);
        BoundingBox(const BoundingBox &other) = default;
        BoundingBox(BoundingBox &&other) = default;
        BoundingBox &operator=(const BoundingBox &other) = default;
        BoundingBox &operator=(BoundingBox &&other) = default;
        ~BoundingBox() = default;

        /**
         * @brief Compare two bounding boxes
         *
         * @param other The other bounding box to compare with
         * @return true If the two bounding boxes have the same position and size
         * @return false If the two bounding boxes don't have the same position and size
         */
        bool operator==(const BoundingBox &other) const;
        /**
         * @brief Compare two bounding boxes
         *
         * @param other The other bounding box to compare with
         * @return true If the two bounding boxes don't have the same position and size
         * @return false If the two bounding boxes have the same position and size
         */
        bool operator!=(const BoundingBox &other) const;

        /**
         * @brief Check if the bounding box collide with another bounding box
         *
         * @param other The other bounding box to check collision with
         * @return true If the two bounding boxes collide
         * @return false If the two bounding boxes don't collide
         */
        bool collide(const BoundingBox &other) const;
        /**
         * @brief Check if the bounding box collide with a point
         *
         * @param pos The position of the point (x, y)
         * @return true If the bounding box collide with the point
         * @return false If the bounding box don't collide with the point
         */
        bool collide(const std::pair<T, T> &pos) const;
        /**
         * @brief Check if the bounding box collide with a point
         *
         * @param x_ The x position of the point
         * @param y_ The y position of the point
         * @return true If the bounding box collide with the point
         * @return false If the bounding box don't collide with the point
         */
        bool collide(T x_, T y_) const;
        /**
         * @brief Check if the bounding box collide with another bounding box
         *
         * @param pos The position of the bounding box (x, y)
         * @param size The size of the bounding box (width, height)
         * @return true If the two bounding boxes collide
         * @return false If the two bounding boxes don't collide
         */
        bool collide(const std::pair<T, T> &pos, const std::pair<T, T> &size) const;
        /**
         * @brief Check if the bounding box collide with another bounding box
         *
         * @param x_ The x position of the bounding box
         * @param y_ The y position of the bounding box
         * @param w_ The width of the bounding box
         * @param h_ The height of the bounding box
         * @return true If the two bounding boxes collide
         * @return false If the two bounding boxes don't collide
         */
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

#endif
