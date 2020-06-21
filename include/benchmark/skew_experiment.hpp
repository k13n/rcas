#ifndef BENCHMARK_SKEW_EXPERIMENT_H_
#define BENCHMARK_SKEW_EXPERIMENT_H_

#include "cas/index.hpp"
#include "cas/cas.hpp"
#include "benchmark/approach.hpp"


namespace benchmark {


template<class VType>
class SkewExperiment {
public:
  struct Dataset {
    size_t cardinality_;
    double skew_;
    std::string filename_;
    VType low_;
    VType high_;
  };

private:
  const std::string& query_path_;
  const std::vector<Approach>& approaches_;
  const std::vector<Dataset>& datasets_;
  const char dataset_delim_;
  std::vector<cas::QueryStats> results_;

public:
  SkewExperiment(
      const std::string& query_path,
      const std::vector<Approach>& approaches,
      const std::vector<Dataset>& datasets,
      const char dataset_delim);

  void Run();

  void RunIndex(cas::Index<VType>& index, int nr_repetitions,
      const Dataset& dataset);

  void PopulateIndex(cas::Index<VType>& index, std::string filename);

  void PrintOutput();

private:
  void SetAllValues(cas::SearchKey<VType>& skey);
};


}; // namespace benchmark


#endif // BENCHMARK_SKEW_EXPERIMENT_H_
