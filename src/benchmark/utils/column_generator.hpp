#include <vector>
#include <random>

template <class ValueType>
std::vector<ValueType> generate_uniform_vector(size_t size, ValueType min_val, ValueType max_val)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<ValueType> dist(min_val, max_val);

    std::vector<ValueType> result;
    result.reserve(size);

    for (size_t i = 0; i < size; ++i)
    {
        result.push_back(dist(gen));
    }

    return result;
}