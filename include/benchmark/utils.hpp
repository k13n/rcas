#ifndef BENCHMARK_UTILS_H_
#define BENCHMARK_UTILS_H_

#include "cas/index.hpp"
#include "benchmark/approach.hpp"


namespace benchmark {

template<class VType>
cas::Index<VType>* CreateIndex(Approach approach);


}; // namespace benchmark


#endif // BENCHMARK_UTILS_H_
