#pragma once

#include <print>
#include <thread>

#include "taskflow/core/executor.hpp"
#include "taskflow/core/semaphore.hpp"
#include "taskflow/core/task.hpp"
#include "taskflow/core/taskflow.hpp"
#include "taskflow/taskflow.hpp"

namespace learning_taskflow {

void run_limited_concurrency() {
    auto sl = []() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    };

    tf::Executor exec;
    tf::Taskflow taskflow;

    tf::Semaphore semaphore(1);
    std::vector<tf::Task> tasks {
        taskflow.emplace([=]() { sl(); std::print("A\n"); }),
        taskflow.emplace([=]() { sl(); std::print("B\n"); }),
        taskflow.emplace([=]() { sl(); std::print("C\n"); }),
        taskflow.emplace([=]() { sl(); std::print("D\n"); }),
        taskflow.emplace([=]() { sl(); std::print("E\n"); })
    };
    for (auto& task : tasks) {
        // 设置 task 需要 acquire和 release 的 semaphore
        task.acquire(semaphore);
        task.release(semaphore);
    }
    exec.run(taskflow);
    exec.wait_for_all();

}

}  // namespace learning_taskflow
