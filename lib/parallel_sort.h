#pragma once

#include <chrono>
#include <print>
#include <random>

#include "taskflow/algorithm/sort.hpp"
#include "taskflow/core/executor.hpp"
#include "taskflow/core/taskflow.hpp"
#include "taskflow/taskflow.hpp"

#include "utils/util.h"

namespace learning_taskflow {

// generating random strings ...
// std::sort ... 403 ms
// generating random strings ...
// std::sort ... 765 ms
// generating random strings ...
// std::sort ... 376 ms
// generating random strings ...
// std::sort ... 375 ms
// generating random strings ...
// std::sort ... 378 ms
// generating random strings ...
// Taskflow Parallel Sort ... 94 ms
// generating random strings ...
// Taskflow Parallel Sort ... 98 ms
// generating random strings ...
// Taskflow Parallel Sort ... 99 ms
// generating random strings ...
// Taskflow Parallel Sort ... 98 ms
// generating random strings ...
// Taskflow Parallel Sort ... 99 ms

std::string random_string(size_t len) {
    std::string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::default_random_engine eng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(1, 100000);

    tmp_s.reserve(len);

    for (size_t i = 0; i < len; ++i) {
        tmp_s += alphanum[dist(eng) % (sizeof(alphanum) - 1)];
    }
    return tmp_s;
}

std::vector<std::string> random_strings() {
    std::vector<std::string> strings(1000000);
    std::print("generating random strings ...\n");
    for (auto& str: strings) {
        str = random_string(32);
    }
    return strings;
}

void run_serial() {
    auto str = random_strings();
    std::print("std::sort ... ");
    auto beg = utils::now();
    std::sort(str.begin(), str.end());
    auto end = utils::now();
    std::print(
        "{} ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count());
}

void run_parallel() {
    auto strs = random_strings();
    std::print("Taskflow Parallel Sort ... ");
    auto beg = utils::now();
    {
        tf::Taskflow taskflow;
        tf::Executor exec;
        taskflow.sort(strs.begin(), strs.end());
        exec.run(taskflow).wait();
    }
    auto end = utils::now();
    std::print(
        "{} ms\n",
        std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count());

}

void run_parallel_sort() {
    for (int i = 0; i < 5; ++i) {
        run_serial();
    }
    for (int i = 0; i < 5; ++i) {
        run_parallel();
    }
}

}
