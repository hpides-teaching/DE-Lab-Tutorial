#include <chrono>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "cxxopts.hpp"
#include "hash_join.hpp"
#include "join.hpp"
#include "nested_loop_join.hpp"
#include "utils/benchmark_runner.hpp"
#include "utils/column_generator.hpp"
#include "utils/runtime_config.hpp"

using value_type = uint32_t;

struct JoinConfig {
  size_t probe_size;
  size_t build_size;
  std::string algo;
};

void benchmark_join(JoinConfig& config, auto& results) {
  auto probe_side = generate_uniform_vector<value_type>(config.probe_size, 0, 100000000);
  auto build_side = generate_uniform_vector<value_type>(config.build_size, 0, 100000000);

  std::unique_ptr<JoinOperator<value_type>> join_operator;

  // Initialize the correct join algorithm
  if (config.algo == "hash") {
    join_operator = std::make_unique<HashJoin<value_type>>(build_side, probe_side);
  } else if (config.algo == "hash_inverted") {
    join_operator = std::make_unique<HashJoin<value_type>>(probe_side, build_side);
  } else if (config.algo == "nested_loop") {
    join_operator = std::make_unique<NestedLoopJoin<value_type>>(build_side, probe_side);
  } else if (config.algo == "nested_loop_inverted") {
    join_operator = std::make_unique<NestedLoopJoin<value_type>>(probe_side, build_side);
  } else {
    throw std::runtime_error("Unknown algo set: " + config.algo);
  }

  BenchmarkRunner::get().perf_wrapper.start();
  auto start = std::chrono::high_resolution_clock::now();
  auto join_count = join_operator->join();
  asm volatile("" : "+r,m"(join_count) : : "memory");  // Please don't optimize away. // NOLINT
  auto end = std::chrono::high_resolution_clock::now();
  BenchmarkRunner::get().perf_wrapper.stop();
  BenchmarkRunner::get().perf_wrapper.result(results, config.probe_size);
  results["runtime"] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int main(int argc, char** argv) {
  auto& benchmark_config = BenchmarkRunner::get().runtime_config;
  BenchmarkRunner::get().perf_wrapper.initialize();

  // clang-format off
    benchmark_config.add_options()
        ("probe_size",
              "Number of elements in probe side",
              cxxopts::value<std::vector<size_t>>()->default_value("10000000"))
        ("build_size",
              "Number of elements in build side",
              cxxopts::value<std::vector<size_t>>()->default_value("100"))
        ("algo",
              "Algorithm to use (hash, hash_inverted, nested_loop, nested_loop_inverted)",
              cxxopts::value<std::vector<std::string>>()->default_value("hash,hash_inverted,nested_loop,nested_loop_inverted")) // NOLINT
        ("out",
              "filename",
              cxxopts::value<std::vector<std::string>>()->default_value("join_benchmark.json"));
  // clang-format on
  benchmark_config.parse(argc, argv);

  std::vector<nlohmann::json> all_results;
  for (auto& runtime_config : benchmark_config.get_runtime_configs()) {
    const auto probe_size = convert<size_t>(runtime_config["probe_size"]);
    const auto build_size = convert<size_t>(runtime_config["build_size"]);
    const auto algo = convert<std::string>(runtime_config["algo"]);
    JoinConfig config = {
        probe_size,
        build_size,
        algo,
    };

    nlohmann::json results;
    results["config"]["probe_size"] = probe_size;
    results["config"]["build_size"] = build_size;
    results["config"]["algo"] = algo;

    benchmark_join(config, results);

    all_results.push_back(results);
    auto results_file = std::ofstream{convert<std::string>(runtime_config["out"])};
    nlohmann::json intermediate_json;
    intermediate_json["results"] = all_results;
    results_file << intermediate_json.dump(-1) << std::flush;
  }
}
