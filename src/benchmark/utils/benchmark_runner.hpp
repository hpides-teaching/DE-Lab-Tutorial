#pragma once

#include <boost/container/pmr/memory_resource.hpp>

#include "utils/singleton.hpp"
#include "runtime_config.hpp"
#include "perf_wrapper.hpp"

class BenchmarkRunner : public Singleton<BenchmarkRunner>
{
public:
    RuntimeConfig runtime_config;
    PerfWrapper perf_wrapper;

private:
    BenchmarkRunner();
    friend class Singleton;
};