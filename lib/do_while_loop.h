#pragma once

#include "taskflow/taskflow.hpp"

#include "utils/graph_visualize.h"

namespace learning_taskflow {

void run_do_while_loop() {
    tf::Executor exec;
    tf::Taskflow taskflow("do_while_loop");
    int i;
    auto init = taskflow.emplace([&]() {
        std::print("i=0\n");
        i = 0;
    });
    init.name("init=0");

    auto body = taskflow.emplace([&]() {
        std::print("++i => i=");
        ++i;
    });
    body.name("do ++i");

    auto cond = taskflow.emplace([&]() {
        std::print("{}\n", i);
        return i < 5 ? 0 : 1;
    });
    cond.name("while i < 5");

    auto done = taskflow.emplace([&]() {
        std::print("done\n");
    });
    done.name("done");

    init.precede(body);
    body.precede(cond);
    cond.precede(body, done);

    utils::visualize(taskflow, "do_while_loop.dot");
    exec.run(taskflow).wait();
}

}
