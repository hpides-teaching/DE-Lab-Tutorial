#pragma once

#include <memory>
#include <vector>

/*
    Basic Join Operator.
    We simplify by only returning the count of the joined columns.
    e.g. join([1, 2, 3], [2, 2, 1]) ->  3
*/
template <class ValueType>
class JoinOperator {
 public:
  JoinOperator(std::shared_ptr<std::vector<ValueType>> build_side, std::shared_ptr<std::vector<ValueType>> probe_side)
      : _build_side(build_side), _probe_side(probe_side) {}

  virtual ~JoinOperator() = default;
  JoinOperator(const JoinOperator&) = delete;
  JoinOperator& operator=(const JoinOperator&) = delete;
  JoinOperator(JoinOperator&& other) noexcept = default;
  JoinOperator& operator=(JoinOperator&& other) noexcept = default;

  virtual uint64_t join() = 0;

  std::shared_ptr<std::vector<ValueType>> _build_side;
  std::shared_ptr<std::vector<ValueType>> _probe_side;
};
