#pragma once

#include <memory>
#include <random>
#include <vector>

template <class ValueType>
std::shared_ptr<std::vector<ValueType>> generate_uniform_vector(size_t size, ValueType min_val, ValueType max_val) {
  std::random_device rd_device;
  std::mt19937 gen(rd_device());

  std::uniform_int_distribution<ValueType> dist(min_val, max_val);

  auto result = std::make_shared<std::vector<ValueType>>();

  result->reserve(size);

  for (size_t i = 0; i < size; ++i) {
    result->push_back(dist(gen));
  }

  return result;
}
