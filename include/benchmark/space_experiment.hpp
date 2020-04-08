#ifndef BENCHMARK_SPACE_EXPERIMENT_H_
#define BENCHMARK_SPACE_EXPERIMENT_H_

#include "cas/index.hpp"
#include "cas/cas.hpp"
#include "cas/search_key.hpp"
#include "benchmark/approach.hpp"


namespace benchmark {


template<class VType>
class SpaceExperiment {
private:
  const std::string dataset_filename_;
  const char dataset_delim_;
  const std::vector<Approach> approaches_;
  std::vector<cas::IndexStats> results_;
  double bytes_per_key_;

public:
  SpaceExperiment(
      const std::string dataset_filename,
      const char dataset_delim,
      const std::vector<Approach> approaches
  );

  void Run();

  void RunIndex(cas::Index<VType>& index);

  void PopulateIndex(cas::Index<VType>& index);

  void PrintOutput();
};


}; // namespace benchmark


#endif // BENCHMARK_SPACE_EXPERIMENT_H_
