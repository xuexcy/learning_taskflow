#pragma once

#include <print>

#include "taskflow/algorithm/pipeline.hpp"
#include "taskflow/core/task.hpp"
#include "taskflow/taskflow.hpp"
#include "utils/graph_visualize.h"

namespace learning_taskflow {
void run_parallel_graph_pipeline() {
    auto f1 = [](auto& node) {
        std::print("f1({})\n", node);
    };
    auto f2 = [](auto& node) {
        std::print("f2({})\n", node);
    };
    auto f3 = [](auto& node) {
        std::print("f3({})\n", node);
    };
    tf::Executor exec;
    tf::Taskflow taskflow("graph processing pipeline");

    const size_t num_lines = 2;
    const std::vector<std::string> nodes{"A", "B", "C", "D", "E"};
    tf::Pipeline pl(
        num_lines,
        tf::Pipe{tf::PipeType::SERIAL, [&](tf::Pipeflow& pf) {
            if (pf.token() == nodes.size()) {
                pf.stop();
            } else {
                f1(nodes[pf.token()]);
            }
        }},
        tf::Pipe{tf::PipeType::SERIAL, [&](tf::Pipeflow& pf) {
            f2(nodes[pf.token()]);
        }},
        tf::Pipe{tf::PipeType::SERIAL, [&](tf::Pipeflow& pf) {
            f3(nodes[pf.token()]);
        }}
    );
    auto init = taskflow.emplace([](){ std::print("ready\n"); }).name("starting pipeline");
    auto task = taskflow.composed_of(pl).name("pipeline");
    auto stop = taskflow.emplace([](){ std::print("stopped\n"); }).name("pipeline stopped");

    init.precede(task);
    task.precede(stop);
    exec.run(taskflow).wait();

    utils::visualize(taskflow, "parallel_graph_pipeline.dot");
}

}  // namespace learning_takflow
