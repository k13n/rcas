#ifndef BENCHMARK_BENCHMARKS_H_
#define BENCHMARK_BENCHMARKS_H_


#include "compile.hpp"
#include "benchmark/skew_experiment.hpp"
#include "benchmark/structure_experiment.hpp"
#include "benchmark/scalability_experiment.hpp"
#include "benchmark/query_experiment.hpp"
#include "benchmark/space_experiment.hpp"
#include "benchmark/cost_experiment.hpp"

#include "cas/cas.hpp"
#include "casxml/casxml.hpp"

#include <iostream>


template<class VType>
cas::SearchKey<VType> Query(std::string path, VType low, VType high) {
  cas::SearchKey<VType> skey;
  skey.path_ = { path };
  skey.low_  = low;
  skey.high_ = high;
  return skey;
}


#endif // BENCHMARK_BENCHMARKS_H_
