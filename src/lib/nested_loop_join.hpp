#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "join.hpp"

template <class ValueType>
class NestedLoopJoin : public JoinOperator<ValueType> {
 public:
  NestedLoopJoin(std::shared_ptr<std::vector<ValueType>> build_side, std::shared_ptr<std::vector<ValueType>> probe_side)
      : JoinOperator<ValueType>(build_side, probe_side) {}

  uint64_t join() override;
};
