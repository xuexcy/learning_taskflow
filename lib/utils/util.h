#pragma once

#include <chrono>

namespace learning_taskflow::utils {

inline auto now() {
    return std::chrono::steady_clock::now();
}

}  // namespace learning_taskflow::utils
