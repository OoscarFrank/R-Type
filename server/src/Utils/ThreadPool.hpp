#pragma once

#include <iostream>
#include <functional>
#include <thread>
#include <future>
#include <atomic>
#include <vector>
#include "Queue.hpp"

class ThreadPool {
    public:
        ThreadPool(int nbrThreads, int secondsToWait);
        ~ThreadPool();
        template<typename T>
        void submit(T func);
        size_t nbrThreads();
        size_t nbrTasks();
    protected:
    private:
        void _deleteThreads();
        void _newThread();
        void _threadHandler();
        Queue<std::function<void()>> _pool;
        std::vector<std::pair<std::thread, std::future<void>>> _threads;
        int _nbrThreads;
        int _secondsToWait;
};

template<typename T>
void ThreadPool::submit(T func)
{
    _pool.push(std::function<void()>(func));
    _deleteThreads();
    if (_threads.empty())
        _newThread();
}
