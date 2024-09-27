cmake_minimum_required(VERSION 3.18)
project (learning_taskflow LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

# https://zhuanlan.zhihu.com/p/687058798
# include taskflow
include(FetchContent)
FetchContent_Declare(
    Taskflow
    GIT_REPOSITORY https://github.com/taskflow/taskflow.git
    GIT_TAG v3.7.0
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/taskflow
)
FetchContent_MakeAvailable(Taskflow)
set(EXTRA_LIBS ${EXTRA_LIBS} Taskflow)

include_directories("${PROJECT_SOURCE_DIR}/lib")
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})
