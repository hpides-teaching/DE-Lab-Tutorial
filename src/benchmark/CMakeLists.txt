include_directories(
    ${PROJECT_SOURCE_DIR}/src/lib
    ${perf-cpp-external_SOURCE_DIR}/include/
)
add_subdirectory(utils)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/benchmark)

add_executable(join_benchmark join_benchmark.cpp)
target_link_libraries(join_benchmark benchmark_runner nested_loop_join hash_join)