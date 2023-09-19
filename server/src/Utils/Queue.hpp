#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#define NOW std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()


template<typename T>
class Queue {
    public:
        Queue() = default;
        ~Queue() = default;

        void push(T &&value);
        bool tryPop(T &value);
        T pop();
        std::size_t size() const { return _queue.size(); }
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
T Queue<T>::pop()
{
    std::unique_lock lock(_mutex);
    if (_queue.empty())
        _cv.wait(lock);
    T value = _queue.front();
    _queue.pop();
    return value;
}
