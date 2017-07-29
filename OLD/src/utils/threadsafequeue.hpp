#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>

using namespace std;

template<class T>
class concurrent_queue {
private:
    queue<T> the_queue;
    mutable std::mutex the_mutex;
    std::condition_variable the_condition_variable;
public:
    void push(T &&data)
    {
        {
            std::lock_guard<std::mutex> lock(the_mutex);
            the_queue.push(data);
        }
        the_condition_variable.notify_one();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(the_mutex);
        return the_queue.empty();
    }

    bool try_pop(T &popped_value)
    {
        std::lock_guard<std::mutex> lock(the_mutex);
        if(the_queue.empty())
        {
            return false;
        }
        
        popped_value = the_queue.front();
        the_queue.pop();
        return true;
    }
};
