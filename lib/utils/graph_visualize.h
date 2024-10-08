#pragma once

#include <expected>
#include <filesystem>
#include <iostream>

#include "taskflow/taskflow.hpp"

namespace learning_taskflow::utils {

std::expected<std::filesystem::path, std::string> get_visualize_path(const std::string& relative_path) {
    static const std::filesystem::path VISUALIZE_DIR{"../task_graph"};
    if (!std::filesystem::exists(VISUALIZE_DIR)) {
        if (!std::filesystem::create_directories(VISUALIZE_DIR)) {
            return std::unexpected(std::format(
                "create visualize directory error, VISUALIZE_DIR: {}",
                VISUALIZE_DIR.string()));
        }
    } else if (!std::filesystem::is_directory(VISUALIZE_DIR)) {
        return std::unexpected(std::format(
            "default visualize directory exists but is a file, VISUALIZE_DIR: {}",
            VISUALIZE_DIR.string()));
    }

    return VISUALIZE_DIR / relative_path;

}
void visualize(
    const tf::Taskflow& tf, const std::string& relative_path,
    std::ostream& os = std::cout) {
    auto graph_str = tf.dump();
    os << graph_str;

    auto path = get_visualize_path(relative_path);
    if (path.has_value()) {
        std::ofstream ofs(*path);
        ofs << graph_str;
        ofs.close();
        std::print("dump taskflow {} to {}\n", tf.name(), (*path).string());
    } else {
        std::print(stderr, "get_visualize_path error, err_msg:[{}]\n", path.error());
    }
}

}
