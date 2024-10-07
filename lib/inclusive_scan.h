#pragma once

#include <numeric>
#include <print>

#include "taskflow/taskflow.hpp"
#include "taskflow/algorithm/scan.hpp"

namespace learning_taskflow {
// W=1, N=20
// sequential inclusive scan 633ns
// parallel inclusive scan 40454ns
// W=1, N=2000
// sequential inclusive scan 42695ns
// parallel inclusive scan 56180ns
// W=1, N=200000
// sequential inclusive scan 3030046ns
// parallel inclusive scan 3469325ns
// W=8, N=20
// sequential inclusive scan 826ns
// parallel inclusive scan 98520ns
// W=8, N=2000
// sequential inclusive scan 38901ns
// parallel inclusive scan 150365ns
// W=8, N=200000
// sequential inclusive scan 3471485ns
// parallel inclusive scan 17788062ns
// W=64, N=20
// sequential inclusive scan 1057ns
// parallel inclusive scan 197112ns
// W=64, N=2000
// sequential inclusive scan 45369ns
// parallel inclusive scan 878276ns
// W=64, N=200000
// sequential inclusive scan 3466139ns
// parallel inclusive scan 1362301ns

void run_inclusive_scan(size_t W, size_t N);

void run_inclusive_scan() {
    run_inclusive_scan(1, 20);
    run_inclusive_scan(1, 2000);
    run_inclusive_scan(1, 200000);
    run_inclusive_scan(8, 20);
    run_inclusive_scan(8, 2000);
    run_inclusive_scan(8, 200000);
    run_inclusive_scan(64, 20);
    run_inclusive_scan(64, 2000);
    run_inclusive_scan(64, 200000);

}

void run_inclusive_scan(size_t W, size_t N) {
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
        std::inclusive_scan(
            elements.begin(), elements.end(), scan_seq.begin()
        );
        // scan_seq: [0, 0 + 1, 0 + 1 + 2, ..., 0 + 1 + 2 + ... + N - 1]
        auto end = std::chrono::steady_clock::now();
        std::print(
            "// sequential inclusive scan {}ns\n",
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count());
    }
    {
        auto beg = std::chrono::steady_clock::now();
        taskflow.inclusive_scan(
            elements.begin(), elements.end(), scan_par.begin(), std::plus<int>()
        );
        exec.run(taskflow).wait();
        auto end = std::chrono::steady_clock::now();
        std::print(
            "// parallel inclusive scan {}ns\n",
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count());

    }
    assert(scan_seq == scan_par);

}

}  // namespace learning_taskflow
