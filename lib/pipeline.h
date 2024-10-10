#pragma once

#include "taskflow/algorithm/pipeline.hpp"
#include "taskflow/core/executor.hpp"
#include "taskflow/core/task.hpp"
#include "taskflow/core/taskflow.hpp"
#include "taskflow/taskflow.hpp"
#include "utils/graph_visualize.h"

namespace learning_taskflow {

void run_pipeline() {
    tf::Taskflow taskflow("pipeline");
    tf::Executor exec;

    const size_t num_lines = 4;
    std::array<size_t, num_lines> buffer;

    tf::Pipeline pl(
        num_lines,
        tf::Pipe{tf::PipeType::SERIAL, [&buffer](tf::Pipeflow& pf) {
            if (5 == pf.token()) {
                pf.stop();
            } else {
                std::print("stage 1: input token = {}\n", pf.token());
                buffer[pf.line()] = pf.token();
            }
        }},
        tf::Pipe{tf::PipeType::PARALLEL, [&buffer](tf::Pipeflow& pf) {
            std::print("stage 2: input buffer[{}] = {}\n", pf.line(), buffer[pf.line()]);
            buffer[pf.line()] = buffer[pf.line()] + 1;
        }},
        tf::Pipe{tf::PipeType::SERIAL, [&buffer](tf::Pipeflow& pf) {
            std::print("stage 3: input buffer[{}] = {}\n", pf.line(), buffer[pf.line()]);
            buffer[pf.line()] = buffer[pf.line()] + 1;
        }}
    );
    tf::Task init = taskflow.emplace([]() { std::print("ready\n"); })
        .name("starting pipeline");
    auto task = taskflow.composed_of(pl);
    auto stop = taskflow.emplace([]() { std::print("stopped\n"); })
        .name("pipeline stopped");
    init.precede(task);
    task.precede(stop);
    exec.run(taskflow).wait();
    utils::visualize(taskflow, "pipeline.dot");
}

}  // namespace learning_taskflow
