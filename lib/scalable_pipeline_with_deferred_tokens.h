#pragma once

#include "taskflow/algorithm/pipeline.hpp"
#include "taskflow/core/executor.hpp"
#include "taskflow/taskflow.hpp"
#include "utils/graph_visualize.h"

// pipe0: 1 2 3

namespace learning_taskflow {

void run_scalable_pipeline_with_deferred_tokens() {
    tf::Taskflow taskflow("pipeline");
    tf::Executor exec;

    const size_t num_lines = 4;
    auto pipe_callable = [](tf::Pipeflow& pf) mutable {
        if (0 == pf.pipe()) {
            if (15 == pf.token()) {
                pf.stop();
            } else if (5 == pf.token()) {
                if (0 == pf.num_deferrals()) {
                    pf.defer(2);
                    std::print("1st-time: Token {} is deferred by 2\n", pf.token());
                    pf.defer(7);
                    std::print("1st-time: Token {} is deferred by 7\n", pf.token());
                } else if (1 == pf.num_deferrals()) {
                    pf.defer(9);
                    std::print("2nd-time: Token {} is deferred by 9\n", pf.token());
                } else if (2 == pf.num_deferrals()) {
                    std::print(
                        "3rd-time: Token 2/7/9 resolved dependencies for token {} is deferred by 9\n",
                        pf.token()
                    );
                }
            } else if (2 == pf.token()) {
                if (0 == pf.num_deferrals()) {
                    pf.defer(8);
                    std::print("1st-time: Token {} is deferred by 8\n", pf.token());
                } else if (1 == pf.num_deferrals()) {
                    std::print("2nd-time: Token 8 resolved dependencies for token {}\n", pf.token());
                }
            } else {
                std::print("stage 1: Non-deferred token {}\n", pf.token());
            }
        } else {
            std::print(
                "stage {}: input token {} (deferrals={})\n",
                pf.pipe() + 1, pf.token(), pf.num_deferrals()
            );
        }
    };

    std::vector<tf::Pipe<std::function<void(tf::Pipeflow&)>>> pipes;
    for (size_t i = 0; i < 3; ++i) {
        pipes.emplace_back(tf::PipeType::SERIAL, pipe_callable);
    }

    tf::ScalablePipeline<decltype(pipes)::iterator> pl(num_lines, pipes.begin(), pipes.end());

    tf::Task init = taskflow.emplace([]() { std::print("ready\n"); })
        .name("staring pipeline");
    auto task = taskflow.composed_of(pl).name("pipeline");
    auto stop = taskflow.emplace([]() { std::print("stopped\n"); })
        .name("pipeline stopped");
    init.precede(task);
    task.precede(stop);

    exec.run(taskflow).wait();
    utils::visualize(taskflow, "scalable_pipeline_with_deferred_tokens.dot");

    for (size_t i = 0; i < 2; ++i) {
        pipes.emplace_back(tf::PipeType::SERIAL, pipe_callable);
    }
    pl.reset(pipes.begin(), pipes.end());
    exec.run(taskflow).wait();
}

}  // namespace learning_taskflow
