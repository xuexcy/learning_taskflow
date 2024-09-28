#pragma once

#include "taskflow/taskflow.hpp"

#include "utils/graph_visualize.h"


namespace learning_taskflow {

void composition_example_1() {
    std::print("Composition example 1\n");
    tf::Executor executor;
    tf::Taskflow f1("F1");
    auto f1A = f1.emplace([]() { std::print("F1 TaskA\n"); });
    auto f1B = f1.emplace([]() { std::print("F1 TaskB\n"); });
    auto f1C = f1.emplace([]() { std::print("F1 TaskC\n"); });
    f1A.name("f1A");
    f1B.name("f1B");
    f1C.name("f1C");

    f1A.precede(f1C);
    f1B.precede(f1C);

    tf::Taskflow f2("F2");
    auto f2A = f2.emplace([](){ std::print("F2 TaskA\n"); });
    auto f2B = f2.emplace([](){ std::print("F2 TaskB\n"); });
    auto f2C = f2.emplace([](){ std::print("F2 TaskC\n"); });
    auto f2D = f2.emplace([](){ std::print("F2 TaskD\n"); });
    f2A.name("f2A");
    f2B.name("f2B");
    f2C.name("f2C");
    f2D.name("f2D");

    f2A.precede(f2C);
    f2B.precede(f2C);

    // f1_module_task:
    // f1A
    //    ----> f1C
    // f1B

    // f2A
    //    ----> f2C ----> f1_module_task ----> f2D
    // f2B
    tf::Task f1_module_task = f2.composed_of(f1);
    f1_module_task.name("module");
    f2C.precede(f1_module_task);
    f1_module_task.precede(f2D);
    utils::visualize(f2, "composition_example_1.dot");
    // run f2 3times
    executor.run_n(f2, 3).get();
}

void composition_example_2() {
    std::print("Composition example 2\n");
    tf::Executor executor;
    tf::Taskflow f1("F1");
    auto f1A = f1.emplace([&](){ std::print("F1 TasksA\n"); });
    auto f1B = f1.emplace([&](){ std::print("F1 TasksB\n"); });
    f1A.name("f1A");
    f1B.name("f1B");
    // f1:
    // f1A
    // f1B


    tf::Taskflow f2("F2");
    auto f2A = f2.emplace([&]() { std::print("F2 TaskA\n"); });
    auto f2B = f2.emplace([&]() { std::print("F2 TaskB\n"); });
    auto f2C = f2.emplace([&]() { std::print("F2 TaskC\n"); });
    f2A.name("f2A");
    f2B.name("f2B");
    f2C.name("f2C");

    f2A.precede(f2C);
    f2B.precede(f2C);
    f2.composed_of(f1).name("module_of_f1");
    // f2:
    // f2A
    //    ----> f2C
    // f2B
    // module_of_f1: f1

    tf::Taskflow f3("F3");
    f3.composed_of(f2).name("module_of_f2");
    f3.emplace([](){ std::print(" F3 TaskA\n"); }).name("f3A");
    // f3:
    // f3A
    // module_of_f2: f2

    tf::Taskflow f4;
    f4.name("F4");
    auto f3_module_task = f4.composed_of(f3).name("module_of_f3");
    auto f2_module_task = f4.composed_of(f2).name("module_of_f2");
    f3_module_task.precede(f2_module_task);
    // f4:
    // module_of_f3 -> module_of_f2

    utils::visualize(f4, "composition_example_2.dot");

    f4.dump(std::cout);
    executor.run_until(
        f4,
        [iter=1]()mutable {
            std::print("\n"); return
            iter-- == 0;
        },
        []() { std::print("First run_until finished\n"); }
    ).get();

    executor.run_until(
        f4,
        [iter = 2] () mutable {
            std::print("\n");
            return iter-- == 0;
        },
        [](){ std::print("Second run_until finished\n"); }
    );

    executor.run_until(
        f4,
        [iter = 3] () mutable {
            std::print("\n");
            return iter-- == 0;
        },
        [](){ std::cout << "Third run_until finished\n"; }
    ).get();
}
void run_composition() {
    composition_example_1();
    composition_example_2();

}

}
