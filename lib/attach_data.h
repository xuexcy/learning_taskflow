#include "taskflow/taskflow.hpp"
namespace learning_taskflow {

void run_attach_data() {
    tf::Executor executor;
    tf::Taskflow taskflow("attach data to a task");
    int data;
    auto A = taskflow.placeholder();
    A.data(&data).work([A]() {
        auto d = *static_cast<int*>(A.data());
        std::cout << "data is " << d << std::endl;
    });
    for (data = 0; data < 10; ++data) {
        executor.run(taskflow).wait();
    }
}

} // namespace learning_taskflow
