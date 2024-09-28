#pragma once

#include "taskflow/taskflow.hpp"

namespace learning_taskflow {

void run_dependent_async() {
    tf::Executor exec;
    std::print("Dependent Async\n");
    auto [A, fuA] = exec.dependent_async([]() {
        std::print("A\n");
    });
    auto [B, fuB] = exec.dependent_async(
        []() {
            std::print("B\n");
        },
        A
    );
    auto [C, fuC] = exec.dependent_async(
        []() {
            std::print("C\n");
        },
        A
    );
    auto [D, fuD] = exec.dependent_async(
        []() {
            std::print("C\n");
        },
        B, C
    );
    fuD.get();
    std::print("Silent Dependent Async\n");
    A = exec.silent_dependent_async([]() {
        std::print("A\n");
    });

    B = exec.silent_dependent_async(
        []() {
            std::print("B\n");
        },
        A
    );
    C = exec.silent_dependent_async(
        []() {
            std::print("C\n");
        },
        A
    );
    D = exec.silent_dependent_async(
        []() {
            std::print("C\n");
        },
        B, C
    );
    exec.wait_for_all();

}

}
