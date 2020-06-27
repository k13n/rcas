#ifndef BENCHMARK_APPROACH_H_
#define BENCHMARK_APPROACH_H_

#include "cas/index_type.hpp"
#include <string>


namespace benchmark {

struct Approach {
  cas::IndexType type_;
  std::string name_;
  bool use_surrogate_;
  size_t max_depth_;
  size_t bytes_per_label_;

  Approach() {};

  Approach(cas::IndexType type, std::string name,
      bool use_surrogate = false,
      size_t max_depth = 0,
      size_t bytes_per_label = 0)
    : type_(type)
    , name_(name)
    , use_surrogate_(use_surrogate)
    , max_depth_(max_depth)
    , bytes_per_label_(bytes_per_label)
  {};
};

}; // namespace benchmark


#endif // BENCHMARK_APPROACH_H_
