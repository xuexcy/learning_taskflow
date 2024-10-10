#pragma once

#include "utils/graph_visualize.h"

#include "taskflow/core/executor.hpp"
#include "taskflow/core/taskflow.hpp"
#include "taskflow/taskflow.hpp"

namespace learning_taskflow {

void run_runtime() {
    tf::Executor exec;
    tf::Taskflow taskflow;

    auto A = taskflow.emplace([]() { return 0; }).name("A");
    auto C = taskflow.emplace([]() { std::print("C\n"); }).name("C");
    auto B = taskflow.emplace([&C](tf::Runtime& rt) {
        std::print("B\n");
        rt.schedule(C);
    }).name("B");
    auto D = taskflow.emplace([]() { std::print("D\n"); }).name("D");

    A.precede(B, C, D);

    exec.run(taskflow).wait();
    utils::visualize(taskflow, "runtime.dot");
}

}  // namespace learning_taskflow
