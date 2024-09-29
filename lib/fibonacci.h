#pragma once

#include "taskflow/taskflow.hpp"

#include "utils/graph_visualize.h"

namespace learning_taskflow
{

int spawn(int n, tf::Subflow& sbf) {
    if (n < 2) { return n; }
    int res1;
    int res2;
    sbf.emplace([&res1, n](tf::Subflow& sbf_n_1) {
        res1 = spawn(n - 1, sbf_n_1);
    }).name(std::to_string(n - 1));
    sbf.emplace([&res2, n](tf::Subflow& sbf_n_2) {
        res2 = spawn(n - 2, sbf_n_2);
    }).name(std::to_string(n - 2));
    sbf.join();
    return res1 + res2;
}

void fibonacci(int N, tf::Executor& exec) {
    auto name = std::format("fibonacci_N{}", N);
    tf::Taskflow taskflow(name);
    int res{0};
    taskflow.emplace([&res, N] (tf::Subflow& sbf) {
        res = spawn(N, sbf);
    }).name(std::to_string(N));
    exec.run(taskflow).wait();
    std::print("{} = {}", name, res);
    utils::visualize(taskflow, std::format("{}.dot", name));
}

void run_fibonacci() {
    tf::Executor exec;
    fibonacci(0, exec);
    fibonacci(1, exec);
    fibonacci(2, exec);
    fibonacci(5, exec);
    fibonacci(10, exec);
}

} // namespace learning_taskflow

