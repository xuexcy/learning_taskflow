#pragma once

#include <filesystem>


namespace learning_taskflow::utils {

std::optional<std::filesystem::path> get_visualize_path(const std::string& relative_path) {
    static const std::filesystem::path VISUALIZE_DIR{"../task_graph"};
    std::cout << "hi";
    if (!std::filesystem::exists(VISUALIZE_DIR)) {
    std::cout << "he";
        if (!std::filesystem::create_directories(VISUALIZE_DIR)) {
    std::cout << "ha";
            std::println(
                stderr, "create visualize directory error, VISUALIZE_DIR: {}",
                VISUALIZE_DIR.string());
            return std::nullopt;
        }
    } else if (!std::filesystem::is_directory(VISUALIZE_DIR)) {
        std::println(
            stderr, "default visualize directory exists but is a file, VISUALIZE_DIR: {}",
            VISUALIZE_DIR.string());
        return std::nullopt;
    }

    return VISUALIZE_DIR / relative_path;

}
void visualize(
    const tf::Taskflow& tf, const std::string& relative_path,
    std::ostream& os = std::cout) {
    auto graph_str = tf.dump();
    os << graph_str;

    if (auto path = get_visualize_path(relative_path)) {
        std::ofstream ofs(path.value());
        ofs << graph_str;
        ofs.close();
    }
}

}
