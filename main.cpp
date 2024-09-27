#include <concepts>
#include <print>

#include "async.h"
#include "attach_data.h"
#include "cancel.h"
#include "composition.h"

#include "taskflow/taskflow.hpp"


template <class F>
requires std::invocable<F>
void invoke(const std::string& name, F& f) {
    std::print("------------ Start {} ------------\n", name);
    f();
    std::print("------------ End {} --------------\n\n", name);
}

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

  #define CONCAT(A, ...) A##__VA_ARGS__
  #define EXPAND(A) #A, A
  #define INVOKE(F) \
    invoke(EXPAND(learning_taskflow::run_##F))
  INVOKE(async);
  INVOKE(attach_data);
  INVOKE(cancel);
  INVOKE(composition);
  #undef CALL
  #undef EXPAND
  #undef CONCAT
  return 0;
}
