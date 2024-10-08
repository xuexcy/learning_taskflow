#pragma once

#include <print>
#include <sstream>
#include <string>

#include "taskflow/core/executor.hpp"
#include "taskflow/core/observer.hpp"
#include "taskflow/core/task.hpp"
#include "taskflow/core/taskflow.hpp"
#include "taskflow/core/worker.hpp"
#include "taskflow/taskflow.hpp"

namespace learning_taskflow {

struct Observer : public tf::ObserverInterface {
    Observer(const std::string& name) {
        std::print("constructing observer {}\n", name);
    }
    void set_up(size_t num_workers) override final {
        std::print("setting up observer with {} workers\n", num_workers);
    }
    void on_entry(tf::WorkerView wv, tf::TaskView tv) override final {
        std::ostringstream oss;
        oss << "worker " << wv.id() << " ready to run " << tv.name() << '\n';
        std::print("{}", oss.str());
    }
    void on_exit(tf::WorkerView wv, tf::TaskView tv) override final {
        std::ostringstream oss;
        oss << "worker " << wv.id() << " finished running " << tv.name() << '\n';
        std::print("{}", oss.str());
    }

};

void run_observer() {
    tf::Executor exec;
    tf::Taskflow taskflow;

    taskflow.emplace([]() { std::print("1\n"); }).name("A");
    taskflow.emplace([]() { std::print("2\n"); }).name("B");
    taskflow.emplace([]() { std::print("3\n"); }).name("C");
    taskflow.emplace([]() { std::print("4\n"); }).name("D");
    taskflow.emplace([]() { std::print("5\n"); }).name("E");
    taskflow.emplace([]() { std::print("6\n"); }).name("F");
    taskflow.emplace([]() { std::print("7\n"); }).name("G");
    taskflow.emplace([]() { std::print("8\n"); }).name("H");

    std::shared_ptr<Observer> observer = exec.make_observer<Observer>("Observer");
    exec.run(taskflow).get();
    exec.remove_observer(std::move(observer));
}
}  // namespace learning_taskflow
