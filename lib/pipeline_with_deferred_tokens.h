#pragma once

#include "taskflow/taskflow.hpp"
#include "utils/graph_visualize.h"

namespace learning_taskflow {

//    ___________
//   |           |
//   V _____     |
//   |     |     |
//   |     V     |
// 1 2 3 4 5 6 7 8 9 10
//         ^   |   |
//         |___|   |
//         ^       |
//         |_______|
//
// 2 is deferred by 8
// 5 is dieferred by 2, 7, and 9

// 1
// 1 2(8)
// 1 2(8) 3
// 1 2(8) 3 4
// 1 2(8) 3 4 5(2, 7)
// 1 2(8) 3 4 5(2, 7) 6
// 1 2(8) 3 4 5(2, 7) 6 7
// 1 2(8) 3 4 5(2, 7) 6 7 8
// 1 2    3 4 5(2, 7) 6 7 8 [2]
// 1 2    3 4 5(7, 9) 6 7 8 [2] [5]
// 1 2    3 4 5(7, 9) 6 7 8 [2] [5] 9
// 1 2    3 4 5(7)    6 7 8 [2] [5] 9 [5]
// 1 2    3 4 5(7)    6 7 8 [2] [5] 9 [5] 10
// 1 2    3 4 5(7)    6 7 8 [2] [5] 9 [5] 10 11
// 1 2    3 4 5(7)    6 7 8 [2] [5] 9 [5] 10 11 12
// 1 2    3 4 5(7)    6 7 8 [2] [5] 9 [5] 10 11 12 13
// 1 2    3 4 5(7)    6 7 8 [2] [5] 9 [5] 10 11 12 13 14

void run_pipeline_with_deferred_tokens() {
    tf::Taskflow taskflow("deferred_pipeline");
    tf::Executor exec;
    const size_t num_lines = 4;

    tf::Pipeline pl(
        num_lines,
        tf::Pipe{tf::PipeType::SERIAL, [](tf::Pipeflow& pf) {
            if (pf.token() == 15) {
                pf.stop();
            } else {
                if (pf.token() == 5) {
                    if (0 == pf.num_deferrals()) {
                        pf.defer(2);
                        std::print("1st-time: Token {} is deferred by 2\n", pf.token());
                        pf.defer(7);
                        std::print("1st-time: Token {} is deferred by 7\n", pf.token());
                        return;
                    } else if (1 == pf.num_deferrals()) {
                        pf.defer(9);
                        std::print("2nd-time: Token {} is deferred by 9\n", pf.token());
                        return;
                    } else if (2 == pf.num_deferrals()) {
                        pf.defer(9);
                        std::print(
                            "3rd-time: Token 2/7/9 resolved dependencies for token {}\n",
                            pf.token());
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
            }
        }},
        tf::Pipe{tf::PipeType::SERIAL, [](tf::Pipeflow& pf) {
            std::print("stage 2: input token {} (deferrals={})\n", pf.token(), pf.num_deferrals());
        }},
        tf::Pipe{tf::PipeType::SERIAL, [](tf::Pipeflow& pf) {
            std::print("stage 3: input token {}\n", pf.token());
        }}
    );
    tf::Task init = taskflow.emplace([]() { std::print("ready\n"); })
        .name("starting pipeline");
    auto task = taskflow.composed_of(pl).name("deferred_pipeline");
    auto stop = taskflow.emplace([]() { std::print("stopped\n"); })
        .name("pipeline stopped");

    init.precede(task);
    task.precede(stop);

    exec.run(taskflow).wait();
    utils::visualize(taskflow, "pipeline_with_deferred_tokens.dot");
}

}
