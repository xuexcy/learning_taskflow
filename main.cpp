#include <functional>
#include <print>

#include "taskflow/taskflow.hpp"

#include "utils/headers_for_run.h"
#include "utils/macros.h"


void invoke(const std::string& name, std::function<void()> f) {
  std::print("------------ Start {} ------------\n", name);
  f();
  std::print("------------ End {} --------------\n\n", name);
}
#define LEARNING_TASKFLOW_INVOKE_EXPAND(A) #A, static_cast<void(*)()>(A)
#define LEARNING_TASKFLOW_INVOKE(F) \
  invoke(LEARNING_TASKFLOW_INVOKE_EXPAND(learning_taskflow::run_##F));

int main(){

  tf::Executor executor;
  tf::Taskflow taskflow;

  auto [A, B, C, D] = taskflow.emplace(  // create four tasks
    [] () { std::cout << "TaskA\n"; },
    [] () { std::cout << "TaskB\n"; },
    [] () { std::cout << "TaskC\n"; },
    [] () { std::cout << "TaskD\n"; }
  );

  A.precede(B, C);  // A runs before B and C
  D.succeed(B, C);  // D runs after  B and C

  executor.run(taskflow).wait();

  FOR_EACH(LEARNING_TASKFLOW_INVOKE,
    async,
    attach_data,
    cancel,
    composition,
    condition,
    corun,
    dependent_async_algorithm,
    dependent_async,
    do_while_loop,
    exception,
    exclusive_scan,
    fibonacci,
    if_else,
    inclusive_scan,
    limited_concurrency,
    multi_condition,
    nested_if_else,
    observer,
    parallel_data_pipeline,
    parallel_for,
    parallel_graph_pipeline,
    parallel_sort,
    pipeline_with_deferred_tokens,
    pipeline,
    priority,
  );
  return 0;
}
