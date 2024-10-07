#pragma once

#include <numeric>

#include "taskflow/taskflow.hpp"
#include "taskflow/algorithm/scan.hpp"

namespace learning_taskflow
{
// W=1, N=2000
// sequential exclusive scan 919ns
// parallel exclusive scan 16873ns
// W=1, N=200000
// sequential exclusive scan 89931ns
// parallel exclusive scan 199650ns
// W=8, N=20
// sequential exclusive scan 144ns
// parallel exclusive scan 41797ns
// W=8, N=2000
// sequential exclusive scan 1103ns
// parallel exclusive scan 37788ns
// W=8, N=200000
// sequential exclusive scan 104183ns
// parallel exclusive scan 193608ns
// W=64, N=20
// sequential exclusive scan 128ns
// parallel exclusive scan 142280ns
// W=64, N=2000
// sequential exclusive scan 1152ns
// parallel exclusive scan 278905ns
// W=64, N=200000
// sequential exclusive scan 107915ns
// parallel exclusive scan 407135ns
void run_exclusive_scan(size_t W, size_t N);
void run_exclusive_scan() {
    run_exclusive_scan(1, 20);
    run_exclusive_scan(1, 2000);
    run_exclusive_scan(1, 200000);
    run_exclusive_scan(8, 20);
    run_exclusive_scan(8, 2000);
    run_exclusive_scan(8, 200000);
    run_exclusive_scan(64, 20);
    run_exclusive_scan(64, 2000);
    run_exclusive_scan(64, 200000);
}

void run_exclusive_scan(size_t W, size_t N) {
    std::print("// W={}, N={}\n", W, N);
    tf::Executor exec(W);
    tf::Taskflow taskflow;

    std::vector<int> elements(N);
    iota(elements.begin(), elements.end(), 0);
    // elements: [0, 1, 2, 3, ..., N - 1]

    std::vector<int> scan_seq(N);
    std::vector<int> scan_par(N);
    {
        auto beg = std::chrono::steady_clock::now();
        std::exclusive_scan(
            elements.begin(), elements.end(), scan_seq.begin(), 0
        );
        // scan_seq: [0, 0 + 0, 0 + 0 + 1, ..., 0 + 1 + 2 + ... + N - 2]
        auto end = std::chrono::steady_clock::now();
        std::print(
            "// sequential exclusive scan {}ns\n",
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count());
    }
    {
        auto beg = std::chrono::steady_clock::now();
        taskflow.exclusive_scan(
            elements.begin(), elements.end(), scan_par.begin(), 0, std::plus<int>()
        );
        exec.run(taskflow).wait();
        auto end = std::chrono::steady_clock::now();
        std::print(
            "// parallel exclusive scan {}ns\n",
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count());

    }
    assert(scan_seq == scan_par);
}

} // namespace learning_taskflow

