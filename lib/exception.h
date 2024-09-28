#pragma once

#include "taskflow/taskflow.hpp"

#include "utils/graph_visualize.h"

namespace learning_taskflow {

void run_exception() {
    tf::Executor exec;
    tf::Taskflow taskflow("exception");
    auto A = taskflow.emplace([]() {
        std::print("TaskA\n");
    });
    A.name("TaskA");

    auto B = taskflow.emplace([]() {
        std::print("TaskB\n");
        throw std::runtime_error("Exception on TaskB");
    });
    B.name("TaskB");

    auto C = taskflow.emplace([]() {
        std::print("TaskC\n");
        throw std::runtime_error("Exception on TaskC");
    });
    C.name("TaskC");

    auto D = taskflow.emplace([]() {
        std::print("TaskD not reach because of exception");
    });
    D.name("D");
    A.precede(B, C);
    D.succeed(B, C);

    utils::visualize(taskflow, "exception.dot");

    try {
        exec.run(taskflow).get();
    } catch(const std::runtime_error& e) {
        std::print("{}\n", e.what());
    }

}
}
