#include "nested_loop_join.hpp"

#include <cstdint>

template <class ValueType>
uint64_t NestedLoopJoin<ValueType>::join() {
  uint64_t matches = 0;

  // Perform nested loop join
  for (const auto& probe_val : *this->_probe_side) {
    for (const auto& build_val : *this->_build_side) {
      if (build_val == probe_val) {
        ++matches;  // Increment match count if values are equal
      }
    }
  }

  return matches;
}

template class NestedLoopJoin<uint32_t>;
