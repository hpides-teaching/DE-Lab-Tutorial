#include "hash_join.hpp"

template <class ValueType>
void HashJoin<ValueType>::build()
{
    for (const auto &val : this->_build_side)
    {
        _value_counts[val]++;
    }
};

template <class ValueType>
unsigned long HashJoin<ValueType>::probe()
{
    unsigned long matches = 0;

    for (const auto &val : this->_probe_side)
    {
        if (_value_counts.find(val) != _value_counts.end())
        {
            matches += _value_counts[val];
        }
    }

    return matches;
};

template <class ValueType>
unsigned long HashJoin<ValueType>::join()
{
    build();
    return probe();
};

template class HashJoin<uint32_t>;