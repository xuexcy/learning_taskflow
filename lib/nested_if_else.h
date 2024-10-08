#pragma once

#include <print>

#include "taskflow/core/executor.hpp"
#include "taskflow/core/taskflow.hpp"
#include "taskflow/taskflow.hpp"
#include "utils/graph_visualize.h"

namespace learning_taskflow {

void run_nested_if_else() {
    tf::Executor exec;
    tf::Taskflow taskflow;

    int i{0};
    auto initi = taskflow.emplace([&]() { i = 3; }).name("init i");
    auto cond1 = taskflow.emplace([&]() { return i > 1 ? 1 : 0; }).name("cond: i > 1");
    auto cond2 = taskflow.emplace([&]() { return i > 2 ? 1 : 0; }).name("cond: i > 2");
    auto cond3 = taskflow.emplace([&]() { return i > 3 ? 1 : 0; }).name("cond: i > 3");
    auto equal1 = taskflow.emplace([&]() { std::print("i=1\n"); }).name("print: i=1");
    auto equal2 = taskflow.emplace([&]() { std::print("i=2\n"); }).name("print: i=2");
    auto equal3 = taskflow.emplace([&]() { std::print("i=3\n"); }).name("print: i=3");
    auto grtr3 = taskflow.emplace([&]() { std::print("i>3\n"); }).name("print: i>3");

    initi.precede(cond1);
    cond1.precede(equal1, cond2);
    cond2.precede(equal2, cond3);
    cond3.precede(equal3, grtr3);
    exec.run(taskflow).wait();
    utils::visualize(taskflow, "nested_if_else.dot");
}

}  // namespace
