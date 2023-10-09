#pragma once

#include <iostream>
#include <functional>
#include <thread>
#include <future>
#include <atomic>
#include <vector>
#include "Queue.hpp"

/**
 * @brief Thread pool class that can execute tasks asynchronously in a limited number of threads
 *
 */
class ThreadPool {
    public:
        /**
         * @brief Construct a new Thread Pool object
         *
         * @param nbrThreads Max number of threads to use
         * @param secondsToWait Seconds to wait before a thread kills itself
         */
        ThreadPool(int nbrThreads, int secondsToWait);
        /**
         * @brief Destroy the Thread Pool object
         *
         */
        ~ThreadPool();
        /**
         * @brief Submit a task to the thread pool
         *
         * @tparam T Type of the task (must be a function or a lambda)
         * @param func Task to submit
         */
        template<typename T>
        void submit(T func);
        /**a
         * @brief Get the number of threads currently running
         *
         * @return size_t
         */
        size_t nbrThreads();
        /**
         * @brief Get the number of tasks currently waiting in the queue
         *
         * @return size_t
         */
        size_t nbrTasks();
    protected:
    private:
        /**
         * @brief Delete threads that have finished their task in the vector
         *
         */
        void _deleteThreads();
        /**
         * @brief Create a new thread
         *
         */
        void _newThread();
        /**
         * @brief Function that runs in each thread
         *
         */
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
