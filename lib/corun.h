#pragma once

#include "taskflow/taskflow.hpp"

namespace learning_taskflow
{

void run_corun() {
    const size_t N = 100;
    const size_t T = 1000;

    tf::Executor executor(2);
    tf::Taskflow taskflow;

    std::array<tf::Taskflow, N> taskflows;
    std::atomic<size_t> counter{0};
    for (size_t n = 0; n < N; ++n) {
        for (size_t i = 0; i < T; ++i) {
            taskflows[n].emplace([&]() { ++counter; });
        }
        taskflow.emplace([&executor, &tf=taskflows[n]]() {
            executor.corun(tf);
        });
    }
    executor.run(taskflow).wait();
    assert(N * T == counter);

}

} // namespace learning_taskflow
