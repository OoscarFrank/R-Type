#include "ThreadPool.hpp"

using namespace std::chrono_literals;

ThreadPool::ThreadPool(int nbrThreads, int secondsToWait): _nbrThreads(nbrThreads), _secondsToWait(secondsToWait)
{
}

ThreadPool::~ThreadPool()
{
    for (auto it = _threads.begin(); it != _threads.end(); ++it) {
        if (it->first.joinable())
            it->first.join();
    }
}

size_t ThreadPool::nbrThreads()
{
    _deleteThreads();
    return _threads.size();
}

size_t ThreadPool::nbrTasks()
{
    return _pool.size();
}

void ThreadPool::_deleteThreads()
{
    for (auto it = _threads.begin(); it != _threads.end();) {
        if (it->second.wait_for(0s) == std::future_status::ready) {
            it->first.join();
            it = _threads.erase(it);
            continue;
        }
        ++it;
    }
}

void ThreadPool::_newThread()
{
    _deleteThreads();
    std::packaged_task<void()> task(std::bind(&ThreadPool::_threadHandler, this));
    std::future<void> future = task.get_future();
    if (static_cast<int>(_threads.size()) < _nbrThreads) {
        _threads.push_back({std::thread(std::move(task)), std::move(future)});
    }
}

void ThreadPool::_threadHandler()
{
    std::cout << "Thread started" << std::endl;
    std::function<void()> func = nullptr;
    while (_pool.tryPop(func, std::chrono::seconds(_secondsToWait))) {
        if (func == nullptr)
            break;
        if (!_pool.empty())
            _newThread();
        func();
        func = nullptr;
    }
    std::cout << "Thread finished" << std::endl;
}
