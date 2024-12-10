#pragma once

#include <perfcpp/event_counter.h>
#include <perfcpp/hardware_info.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

class PerfWrapper {
 public:
  PerfWrapper();
  void initialize();
  void start();
  void stop();
  perf::CounterResult result(std::uint64_t normalization = 1U);
  void result(nlohmann::json& results, std::uint64_t normalization = 1U);
  void result(nlohmann::json& results, const std::string& description, std::uint64_t normalization = 1U);

 private:
  std::optional<perf::EventCounter> event_counter;
  perf::CounterDefinition counter_definition;
  // some additional counters are set in the initialization, depending on system
  // support
  std::vector<std::string> default_counters = {
      "cycles",
      "cpu-clock",
      "instructions",
      "cache-misses",
      "dTLB-load-misses",
      "page-faults",
      "alignment-faults",
      "cache-references",
      "L1-dcache-loads",
      "L1-dcache-load-misses",
      "L1-data-miss-ratio",
      "cycles-per-instruction",
  };
};
