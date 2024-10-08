#include <functional>
#include <print>

#include "async.h"
#include "attach_data.h"
#include "cancel.h"
#include "composition.h"
#include "condition.h"
#include "corun.h"
#include "dependent_async_algorithm.h"
#include "dependent_async.h"
#include "do_while_loop.h"
#include "exception.h"
#include "exclusive_scan.h"
#include "fibonacci.h"
#include "if_else.h"
#include "inclusive_scan.h"
#include "limited_concurrency.h"
#include "multi_condition.h"
#include "utils/macros.h"

#include "taskflow/taskflow.hpp"

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
  );
  return 0;
}
