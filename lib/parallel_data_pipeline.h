#pragma once

#include <print>
#include <string>

#include "taskflow/algorithm/data_pipeline.hpp"
#include "taskflow/algorithm/pipeline.hpp"
#include "taskflow/core/executor.hpp"
#include "taskflow/core/taskflow.hpp"
#include "taskflow/taskflow.hpp"

#include "utils/graph_visualize.h"

namespace learning_taskflow {

void run_parallel_data_pipeline() {
    tf::Taskflow taskflow("pipeline");
    tf::Executor exec;
    const size_t num_lines{3};

    tf::DataPipeline pl(
        num_lines,
        tf::make_data_pipe<void, int>(tf::PipeType::SERIAL, [&](tf::Pipeflow& pf) {
            if (pf.token() == 5) {
                pf.stop();
                return 0;
            } else {
                std::print("first pipe returns {}\n", pf.token());
                return static_cast<int>(pf.token());
            }
        }),
        tf::make_data_pipe<int, std::string>(tf::PipeType::SERIAL, [](int& input) {
            auto res = input + 100;
            std::print("second pipe returns a strong of {}\n", res);
            return std::to_string(res);
        }),
        tf::make_data_pipe<std::string, void>(tf::PipeType::SERIAL, [](std::string& input) {
            std::print("third pipe receives the input string {}\n", input);
        })
    );

    taskflow.composed_of(pl).name("pipeline");
    utils::visualize(taskflow, "parallel_data_pipeline.dot");
    exec.run(taskflow).wait();

}

}  // namespace learning_taskflow
