#pragma once
#include <vector>

/*
    Basic Join Operator.
    We simplify by only returning the count of the joined columns.
    e.g. join([1, 2, 3], [2, 2, 1]) ->  3
*/
template <class ValueType>
class JoinOperator
{
public:
    JoinOperator(std::vector<ValueType> &build_side,
                 std::vector<ValueType> &probe_side)
        : _build_side(build_side), _probe_side(probe_side) {};
    virtual unsigned long join();

    std::vector<ValueType> &_build_side;
    std::vector<ValueType> &_probe_side;
};