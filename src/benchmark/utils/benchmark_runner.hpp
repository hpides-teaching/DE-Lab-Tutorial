#pragma once

#include <boost/container/pmr/memory_resource.hpp>

#include "perf_wrapper.hpp"
#include "runtime_config.hpp"
#include "utils/singleton.hpp"

class BenchmarkRunner : public Singleton<BenchmarkRunner> {
 public:
  RuntimeConfig runtime_config;
  PerfWrapper perf_wrapper;
};
