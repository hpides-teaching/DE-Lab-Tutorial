#include <cstdint>

#include "join.hpp"

template <class ValueType>
class NestedLoopJoin : public JoinOperator<ValueType>
{
public:
    NestedLoopJoin(std::vector<ValueType> &build_side,
                   std::vector<ValueType> &probe_side) : JoinOperator<ValueType>(build_side, probe_side) {};
    unsigned long join() override;
};