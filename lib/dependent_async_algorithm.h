#pragma once

#include "taskflow/taskflow.hpp"
#include "taskflow/algorithm/for_each.hpp"
#include "taskflow/algorithm/reduce.hpp"
#include "taskflow/algorithm/transform.hpp"

namespace learning_taskflow
{

void run_dependent_async_algorithm() {
    const size_t N = 6;
    tf::Executor executor;
    std::vector<int> data(N);

    tf::AsyncTask A = executor.silent_dependent_async(
        tf::make_for_each_task(
            data.begin(), data.end(), [](int& i) { i = 1; std::cout<<"hi\n"; }
        )
    );
    std::cout << "he\n";
    // data: [1; N]


    tf::AsyncTask B = executor.silent_dependent_async(
        tf::make_transform_task(
            data.begin(), data.end(), data.begin(), [](int& i) {
                return i * 2;
            }
        ),
        A
    );
    std::cout << "ha\n";
    // data: [2: N]

    int sum{1};
    tf::AsyncTask C = executor.silent_dependent_async(
        tf::make_reduce_task(
            data.begin(), data.end(), sum, std::plus<int>{}
        ),
        B
    );
    std::cout << "hh\n";
    // sum = sum + std::plus<int>(data) = 1 + 2 * N
    executor.wait_for_all();
    assert(N * 2 + 1 == sum);
}

} // namespace learning_taskflow

