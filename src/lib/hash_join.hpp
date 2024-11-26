#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "join.hpp"

template <class ValueType>
class HashJoin : public JoinOperator<ValueType> {
 public:
  HashJoin(std::shared_ptr<std::vector<ValueType>> build_side, std::shared_ptr<std::vector<ValueType>> probe_side)
      : JoinOperator<ValueType>(build_side, probe_side) {}

  void build();
  uint64_t probe();
  uint64_t join() override;

 private:
  std::unordered_map<ValueType, unsigned> _value_counts;
};
