#pragma once

#include "taskflow/taskflow.hpp"

#include "utils/util.h"

namespace learning_taskflow {

void run_cancel() {
    tf::Executor executor;
    tf::Taskflow taskflow("cancel");
    for (int i = 0; i < 1000; ++i) {
        taskflow.emplace([]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    }
    auto beg = now();
    tf::Future fu = executor.run(taskflow);

    fu.cancel();
    fu.get();
    auto end = now();
    std::cout << "taskflow completes in "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count()
        << "  milliseconds\n";
}

}
