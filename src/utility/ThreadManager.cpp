/// (c) Ben Jones 2019 - present

#include "ThreadManager.hpp"

namespace arrow {
    std::vector<std::thread> ThreadManager::m_threads;
    void ThreadManager::create(std::function<void()> f)
    {
        m_threads.emplace_back(std::move(f));
    }
    void ThreadManager::joinAll()
    {
        for(auto & t : m_threads) {
            t.join();
        }
    }
    void ThreadManager::clear()
    {
        m_threads.clear();
    }
}