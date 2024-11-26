#include "perf_wrapper.hpp"

#include <perfcpp/config.h>
#include <perfcpp/counter.h>
#include <perfcpp/event_counter.h>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

PerfWrapper::PerfWrapper() = default;

// Some possible edge cases encountered on DE-Lab that could be handled in constructor:
// if (!perf::HardwareInfo::is_intel())
//{
//     default_counters.push_back("stalled-cycles-backend");
// }
//  if (!hostname.starts_with("ca"))
//{
//      default_counters.push_back("dTLB-loads");
//      default_counters.push_back("dTLB-miss-ratio");
//  }

void PerfWrapper::initialize() {
  if (event_counter.has_value()) {
    throw std::runtime_error("PerfWrapper has already been initialized.");
  }

  auto config = perf::Config();
  config.include_child_threads(true);  // profiles for all child threads as well.
  config.max_groups(15);

  event_counter.emplace(counter_definition, config);

  if (default_counters.size() > static_cast<size_t>(config.max_counters_per_group()) * config.max_groups()) {
    throw std::runtime_error("Too many perf counters have been defined");
  }

  if (!event_counter.value().add(default_counters)) {
    throw std::runtime_error("Unable to load default Counters.");
  }
}

// Creates the perf events/resets the counters and starts them
void PerfWrapper::start() {
  if (!event_counter.has_value()) {
    throw std::runtime_error(
        "PerfWrapper::start() has been called before "
        "initialize() has been called.");
  }
  try {
    event_counter.value().start();
  } catch (std::runtime_error& e) {
    std::cout << e.what() << std::endl;
    std::cout << "Check for defined counter support on the used system!" << std::endl;
    throw std::runtime_error("Something went wrong during profiling start");
  }
}

void PerfWrapper::stop() {
  if (!event_counter.has_value()) {
    throw std::runtime_error(
        "PerfWrapper::stop() has been called before "
        "initialize() has been called.");
  }
  event_counter.value().stop();
}

perf::CounterResult PerfWrapper::result(std::uint64_t normalization) {
  if (!event_counter.has_value()) {
    throw std::runtime_error(
        "PerfWrapper::result() has been called before "
        "initialize() has been called.");
  }
  return event_counter.value().result(normalization);
}

void PerfWrapper::result(nlohmann::json& results, std::uint64_t normalization) {
  result(results, "", normalization);
}

void PerfWrapper::result(nlohmann::json& results, const std::string& description, std::uint64_t normalization) {
  auto counter_result = result(normalization);
  for (const auto& [counter_name, counter_value] : counter_result) {
    const std::string string_counter_name(counter_name);
    if (description.empty()) {
      results["perf"][string_counter_name] = counter_value;
    } else {
      results["perf"][description][string_counter_name] = counter_value;
    }
  }
}
