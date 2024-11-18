#include <cstdint>
#include <unordered_map>

#include "join.hpp"

template <class ValueType>
class HashJoin : public JoinOperator<ValueType> {
 public:
  HashJoin(std::vector<ValueType>& build_side, std::vector<ValueType>& probe_side)
      : JoinOperator<ValueType>(build_side, probe_side) {}

  void build();
  unsigned long probe();
  unsigned long join() override;

 private:
  std::unordered_map<ValueType, unsigned> _value_counts;
};