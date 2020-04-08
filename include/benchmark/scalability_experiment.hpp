#ifndef BENCHMARK_SCALABILITY_EXPERIMENT_H_
#define BENCHMARK_SCALABILITY_EXPERIMENT_H_

#include "cas/index.hpp"
#include "cas/cas.hpp"
#include "benchmark/approach.hpp"


namespace benchmark {


class ScalabilityExperiment {
public:
  struct Dataset {
    size_t size_;
    std::string filename_;
  };

private:
  const std::vector<Dataset> datasets_;
  const char dataset_delim_;
  std::vector<Approach> approaches_;
  std::vector<cas::IndexStats> results_;
  std::vector<uint64_t> load_times_;

public:
  ScalabilityExperiment(
      const std::vector<Dataset> datasets,
      const char dataset_delim,
      const std::vector<Approach> approaches);

  void Run();

  void RunIndex(cas::Index<cas::vint32_t>& index, const Dataset& dataset);

  uint64_t PopulateIndex(cas::Index<cas::vint32_t>& index, std::string filename);

  void PrintOutput();

  void PrintTableSpace();

  void PrintTableSpacePerKey();

  void PrintTableTime();
};


}; // namespace benchmark


#endif // BENCHMARK_SCALABILITY_EXPERIMENT_H_
