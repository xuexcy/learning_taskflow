#pragma once

#include <chrono>
#include <limits>
#include <random>

#include "taskflow/algorithm/reduce.hpp"
#include "taskflow/core/executor.hpp"
#include "taskflow/core/task.hpp"
#include "taskflow/taskflow.hpp"

#include "utils/graph_visualize.h"
#include "utils/util.h"

namespace learning_taskflow {

// Benchmark: reduce
// [sequential] reduce: 728718 us
// [taskflow] reduce: 103672 us

// Benchmark: transform_reduce
// [sequential] transform_reduce: 595359 us
// [taskflow] transform_reduce: 103188 us

#define MAX_DATA_SIZE 40000000

void run_reduce_min();
void run_transform_reduce_min();

void run_reduce() {
    run_reduce_min();
    run_transform_reduce_min();
}

struct Data {
    int a{::rand()};
    int b{::rand()};
    int transform() const {
        return a * a + 2 * a * b + b * b;
    }
};
void run_reduce_min() {
    std::print("// Benchmark: reduce\n");
    std::vector<int> data;
    data.reserve(MAX_DATA_SIZE);
    for (int i = 0; i < MAX_DATA_SIZE; ++i) {
        data.push_back(::rand());
    }

    auto beg = utils::now();
    auto min = std::numeric_limits<int>::max();
    for (auto& d: data) {
        min = std::min(min, d);
    }
    auto end = utils::now();
    std::print(
        "// [sequential] reduce: {} us\n",
        std::chrono::duration_cast<std::chrono::microseconds>(end - beg).count());

    tf::Taskflow taskflow;
    tf::Executor exec;
    auto tbeg = utils::now();
    auto tmin = std::numeric_limits<int>::max();
    taskflow.reduce(
        data.begin(), data.end(),
        tmin,
        [](int& l, const auto& r) { return std::min(l, r); }
    );
    exec.run(taskflow).get();
    auto tend = utils::now();
    std::print(
        "// [taskflow] reduce: {} us\n",
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tbeg).count());

    assert(min == tmin);
}

void run_transform_reduce_min() {
    std::print("// Benchmark: transform_reduce\n");
    std::vector<Data> data(MAX_DATA_SIZE);
    auto beg = utils::now();
    auto min = std::numeric_limits<int>::max();
    for(auto& d: data) {
        min = std::min(min, d.transform());
    }
    auto end = utils::now();
    std::print(
        "// [sequential] transform_reduce: {} us\n",
        std::chrono::duration_cast<std::chrono::microseconds>(end - beg).count());

    tf::Executor exec;
    tf::Taskflow tf;
    auto tbeg = utils::now();
    auto tmin = std::numeric_limits<int>::max();
    tf.transform_reduce(
        data.begin(), data.end(),
        tmin,
        [](int l, int r) { return std::min(l, r); },
        [](const Data& d) { return d.transform(); }
    );
    exec.run(tf).get();
    auto tend = utils::now();
    std::print(
        "// [taskflow] transform_reduce: {} us\n",
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tbeg).count());
    assert(min == tmin);

}

}  // namespace learning_taskflow
