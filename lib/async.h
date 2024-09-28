#pragma once


#include "taskflow/taskflow.hpp"

#include "utils/graph_visualize.h"

namespace learning_taskflow {
void run_async() {
    tf::Executor executor;
    std::future<int> fu = executor.async([]() {
        std::cout << "async task 1 returns 1\n";
        return 1;
    });

    executor.silent_async([]() {
        std::cout << "async task 2 does not return (silent)\n";
    });
    executor.wait_for_all();

    tf::Taskflow taskflow;
    std::atomic<int> counter {0};
    taskflow.emplace([&](tf::Subflow& sf) {
        for (int i = 0; i < 100; ++i) {
            sf.silent_async([&]() {
                counter.fetch_add(1, std::memory_order_relaxed);
            });
        }
        sf.join();
        if (counter == 100) {
            std::cout << "async tasks spawned from subflow\n";
        } else {
            throw std::runtime_error("this should not happen");
        }
    });
    executor.run(taskflow).wait();
}

}  // namespace learning_taskflow
