#include "threadpool.h"
#include <thread>
#include <algorithm>

namespace bengine {
    std::unique_ptr<ctpl::thread_pool> thread_pool;

    void init_thread_pool() {
        //thread_pool = std::make_unique<ctpl::thread_pool>(std::max((unsigned int)2, std::thread::hardware_concurrency()-1));
        thread_pool = std::make_unique<ctpl::thread_pool>(1);
    }
}
