#pragma once

#include "taskflow/taskflow.hpp"

namespace learning_taskflow
{

void run_condition() {
    tf::Executor executor;
    tf::Taskflow taskflow("Conditional Task Demo");

    int counter;
    auto A = taskflow.emplace([&]() {
        std::print("initializes the counter to zero\n");
        counter = 0;
    }).name("A");
    auto B = taskflow.emplace([&]() {
        std::print("loops to increment the counter\n");
        ++counter;
    }).name("B");

    auto C = taskflow.emplace([&]() {
        std::print("counter is {} ->", counter);
        if (counter != 5) {
            std::print("loops again (goes to B)\n");
            return 0;
        }
        std::print("break the loop (goes to D)\n");
        return 1;
    }).name("C");
    auto D = taskflow.emplace([&]() {
        std::print("done with counter equal to {}\n", counter);
    }).name("D");

    A.precede(B);
    B.precede(C);
    // 0 to B, 1 to D
    C.precede(B);
    C.precede(D);
    // or
    // C.precede(B, D);
    utils::visualize(taskflow, "condition.dot");
    executor.run(taskflow).wait();
    assert(counter == 5);
}

} // namespace learning_taskflow

