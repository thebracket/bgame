#include "threadpool.h"

namespace bengine {
    std::unique_ptr<ctpl::thread_pool> thread_pool;

    void init_thread_pool() {
        thread_pool = std::make_unique<ctpl::thread_pool>(4);
    }
}
