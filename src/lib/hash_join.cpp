#include "hash_join.hpp"

#include <cstdint>

template <class ValueType>
void HashJoin<ValueType>::build() {
  for (const auto& val : *this->_build_side) {
    _value_counts[val]++;
  }
}

template <class ValueType>
uint64_t HashJoin<ValueType>::probe() {
  uint64_t matches = 0;

  for (const auto& val : *this->_probe_side) {
    if (_value_counts.find(val) != _value_counts.end()) {
      matches += _value_counts[val];
    }
  }

  return matches;
}

template <class ValueType>
uint64_t HashJoin<ValueType>::join() {
  build();
  return probe();
}

template class HashJoin<uint32_t>;
