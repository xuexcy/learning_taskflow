#pragma once

#include <print>

#include "taskflow/core/executor.hpp"
#include "taskflow/core/task.hpp"
#include "taskflow/core/tsq.hpp"
#include "taskflow/taskflow.hpp"

namespace learning_taskflow {

void run_priority() {
    tf::Executor exec(1);
    tf::Taskflow taskflow;

    int counter{0};
    auto A = taskflow.emplace([](){});
    auto B = taskflow.emplace([&](){ std::print("Task B: {}\n", counter++); });
    auto C = taskflow.emplace([&](){ std::print("Task C: {}\n", counter++); });
    auto D = taskflow.emplace([&](){ std::print("Task D: {}\n", counter++); });
    auto E = taskflow.emplace([](){});

    A.precede(B, C, D);
    E.succeed(B, C, D);

    B.priority(tf::TaskPriority::HIGH);
    C.priority(tf::TaskPriority::LOW);
    D.priority(tf::TaskPriority::NORMAL);

    // A -> B -> D -> E -> E
    // Task B: 0
    // Task D: 1
    // Task C: 2
    exec.run(taskflow).wait();
}

}  // namespace learning_taskflow
