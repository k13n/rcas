#ifndef BENCHMARK_STRUCTURE_EXPERIMENT_H_
#define BENCHMARK_STRUCTURE_EXPERIMENT_H_

#include "cas/index.hpp"
#include "cas/cas.hpp"
#include "benchmark/approach.hpp"


namespace benchmark {


template<class VType>
class StructureExperiment {
public:
  struct Dataset {
    size_t cardinality_;
    double skew_;
    std::string filename_;
  };

private:
  const std::string dataset_;
  const char dataset_delim_;

public:
  StructureExperiment(
      const std::string& dataset,
      const char dataset_delim);

  void Run();

  void PopulateIndex(cas::Index<VType>& index);

  void PrintOutput(const cas::IndexStats& stats);
};


}; // namespace benchmark


#endif // BENCHMARK_STRUCTURE_EXPERIMENT_H_
