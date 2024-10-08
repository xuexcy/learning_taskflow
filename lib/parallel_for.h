#pragma once

#include <print>

#include "taskflow/core/executor.hpp"
#include "taskflow/core/task.hpp"
#include "taskflow/core/taskflow.hpp"
#include "taskflow/taskflow.hpp"
#include "utils/graph_visualize.h"

namespace learning_taskflow {

void for_each(int N);
void for_each_index(int N);

void run_parallel_for() {
    for_each(1);
    for_each(4);
    for_each(16);
    for_each(64);
    for_each_index(1);
    for_each_index(4);
    for_each_index(16);
    for_each_index(64);

}

void for_each(int N) {
    tf::Executor exec;
    tf::Taskflow taskflow(std::format("for_each_{}", N));

    std::vector<int> range(N);
    std::iota(range.begin(), range.end(), 0);
    taskflow.for_each(range.begin(), range.end(), [&](int i) {
        std::print("for_each on container item: {}\n", i);
    }).name(std::format("for_each_0_to_{}", N - 1));
    exec.run(taskflow).get();
    utils::visualize(taskflow, std::format("parallel_for_each_{}.dot", N));
}

void for_each_index(int N) {
    tf::Executor exec;
    tf::Taskflow taskflow(std::format("for_each_index_{}", N));
    taskflow.for_each_index(0, N, 2, [](int i) {
        std::print("for_each_index on index: {}\n", i);
    }).name(std::format("for_each_index_0_to_{}", N - 1));
    exec.run(taskflow).get();
    utils::visualize(taskflow, std::format("parallel_for_each_index_{}.dot", N));
}

}  // namespace learning_taskflow
