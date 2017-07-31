#pragma once

#include <memory>
#include "ctpl.h"

namespace bengine {
    extern std::unique_ptr<ctpl::thread_pool> thread_pool;

    void init_thread_pool();
}