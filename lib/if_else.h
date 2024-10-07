#pragma once

#include <print>

#include "taskflow/taskflow.hpp"

#include "utils/graph_visualize.h"



namespace learning_taskflow {

void run_if_else() {
    tf::Executor exec;
    tf::Taskflow taskflow;

    auto [init, cond, yes, no] = taskflow.emplace(
        []() {},
        []() { return 0; },
        []() { std::print("yes\n"); },
        []() { std::print("no\n"); }
    );
    init.name("init");
    cond.name("cond");
    yes.name("yes");
    no.name("no");

    cond.succeed(init);
    cond.precede(yes, no);

    utils::visualize(taskflow, "if_else.dot");
    exec.run(taskflow).wait();
}

}  // namespace learning_taskflow
