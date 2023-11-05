#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#define NOW std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()


/**
 * @brief Queue class that can be used to store a list of things to do and share it between threads
 *
 * @tparam T Type of data to store
 */
template<typename T>
class Queue {
    public:
        Queue() = default;
        ~Queue() = default;

        /**
         * @brief Push a new value to the queue
         *
         * @param value Value to add at the end of the queue
         */
        void push(T &&value);
        /**
         * @brief Try to pop a value from the queue if there is one
         *
         * @param value Value where the popped value will be stored
         * @return true If a value has been popped and the queue is not empty
         * @return false If the queue is empty
         */
        bool tryPop(T &value);
        /**
         * @brief Try to pop a value from the queue if there is one and wait for a certain amount of time if there is none
         *
         * @tparam Rep Type of the timeOut duration
         * @tparam Period Type of the timeOut duration
         * @param value Value where the popped value will be stored
         * @param timeOut Duration to wait if there is no value in the queue
         * @return true If a value has been popped and the queue is not empty
         * @return false If the queue is empty after the timeOut duration
         */
        template<typename Rep, typename Period>
        bool tryPop(T &value, const std::chrono::duration<Rep, Period> &timeOut);
        /**
         * @brief Pop a value from the queue and wait if there is none
         *
         * @return T
         */
        T pop();
        /**
         * @brief Get the size of the queue
         *
         * @return std::size_t
         */
        std::size_t size() const { return _queue.size(); }
        /**
         * @brief Check if the queue is empty
         *
         * @return true
         * @return false
         */
        bool empty() const { return _queue.empty(); }

    private:
        std::mutex _mutex;
        std::condition_variable _cv;
        std::queue<T> _queue;
};

template<typename T>
void Queue<T>::push(T &&value)
{
    std::unique_lock lock(_mutex);
    _queue.push(std::move(value));
    _cv.notify_one();
}

template<typename T>
bool Queue<T>::tryPop(T &value)
{
    std::unique_lock lock(_mutex);
    if (_queue.empty())
        return false;
    value = std::move(_queue.front());
    _queue.pop();
    return true;
}

template<typename T>
template<typename Rep, typename Period>
bool Queue<T>::tryPop(T &value, const std::chrono::duration<Rep, Period> &timeOut)
{
    std::unique_lock lock(_mutex);
    if (_queue.empty())
        if (_cv.wait_for(lock, std::chrono::seconds(10), [this]() { return !_queue.empty(); }) == false)
            return false;
    value = std::move(_queue.front());
    _queue.pop();
    return true;
}

template<typename T>
T Queue<T>::pop()
{
    std::unique_lock lock(_mutex);
    if (_queue.empty())
        _cv.wait(lock, [this]() { return !_queue.empty(); });
    T value = std::move(_queue.front());
    _queue.pop();
    return value;
}

#endif
