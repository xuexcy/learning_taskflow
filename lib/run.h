#pragma once

#include "taskflow/core/task.hpp"
#include "taskflow/taskflow.hpp"
#include "utils/graph_visualize.h"

namespace learning_taskflow {

void run_run() {
    tf::Executor exec(1);
    tf::Taskflow taskflow("Demo");

    auto A = taskflow.emplace([&]() { std::print("TaskA\n"); }).name("A");
    auto B = taskflow.emplace([&](tf::Subflow& subflow) {
        std::print("TaskB\n");
        auto B1 = subflow.emplace([&]() { std::print("TaskB1\n"); }).name("B1");
        auto B2 = subflow.emplace([&]() { std::print("TaskB2\n"); }).name("B2");
        auto B3 = subflow.emplace([&]() { std::print("TaskB3\n"); }).name("B3");
        B1.precede(B2);
        B2.precede(B3);
    }).name("B");
    auto C = taskflow.emplace([&](){ std::print("TaskC\n"); }).name("C");
    auto D = taskflow.emplace([&](){ std::print("TaskD\n"); }).name("D");
    A.precede(B, C);
    D.succeed(B, C);

    // visualize without subflow
    utils::visualize(taskflow, "run_no_subflow.dot");
    exec.run(taskflow).get();
    utils::visualize(taskflow, "run_has_subflow.dot");

    exec.run(taskflow).get();
    exec.wait_for_all();

    exec.run(taskflow).get();
    utils::visualize(taskflow, "run_twice.dot");

    exec.run_n(
        taskflow, 4,
        []() { std::print("finished 4 runs\n"); }).get();
    exec.run_until(taskflow, [counter=3]() mutable {
        std::print("Counter = {}\n", counter);
        return counter -- == 0;
    });
    utils::visualize(taskflow, "run_run_until.dot");


}

}  // namespace learning_taskflow
