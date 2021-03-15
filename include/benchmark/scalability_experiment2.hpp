#ifndef BENCHMARK_SCALABILITY_EXPERIMENT2_H_
#define BENCHMARK_SCALABILITY_EXPERIMENT2_H_

#include "cas/index.hpp"
#include "cas/cas.hpp"
#include "benchmark/approach.hpp"


namespace benchmark {


class ScalabilityExperiment2 {
public:
  struct Dataset {
    std::string filename_;
    size_t page_size_;
    size_t dataset_size_;
  };

private:
  const std::vector<Dataset> datasets_;
  std::vector<cas::IndexStats> results_;
  std::vector<uint64_t> load_times_;

public:
  ScalabilityExperiment2(
      const std::vector<Dataset>& datasets);

  void Run();

  void RunIndex(cas::Cas<cas::vint64_t>& index, const Dataset& dataset);

  uint64_t PopulateIndex(cas::Cas<cas::vint64_t>& index, const Dataset& dataset) const;

  void PrintOutput();
};


}; // namespace benchmark


#endif // BENCHMARK_SCALABILITY_EXPERIMENT2_H_
