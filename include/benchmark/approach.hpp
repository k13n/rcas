#ifndef BENCHMARK_APPROACH_H_
#define BENCHMARK_APPROACH_H_

#include "cas/index_type.hpp"


namespace benchmark {

struct Approach {
  cas::IndexType type_;
  std::string name_;
  bool use_surrogate_;
  size_t max_depth_;
  size_t bytes_per_label_;
};

}; // namespace benchmark


#endif // BENCHMARK_APPROACH_H_
