#pragma once

#include <print>

#include "taskflow/core/executor.hpp"
#include "taskflow/core/taskflow.hpp"
#include "taskflow/taskflow.hpp"
#include "taskflow/utility/small_vector.hpp"
#include "utils/graph_visualize.h"

namespace learning_taskflow {

void run_multi_condition() {
    tf::Executor exec;
    tf::Taskflow taskflow("multi-conditional task");
    auto A = taskflow.emplace([&]() -> tf::SmallVector<int> {
        std::print("A\n");
        return {0, 2};
    });
    auto B = taskflow.emplace([&]() { std::print("B\n"); }).name("B");
    auto C = taskflow.emplace([&]() { std::print("C\n"); }).name("C");
    auto D = taskflow.emplace([&]() { std::print("D\n"); }).name("D");
    // condition:
    // 0 -> B
    // 1 -> C
    // 2 -> D
    A.precede(B, C, D);
    exec.run(taskflow).wait();
    utils::visualize(taskflow, "multi_condition.dot");
}

}  // namespace learning_taskflow
