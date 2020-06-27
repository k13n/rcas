#include "benchmark/query_experiment.hpp"
#include "benchmark/utils.hpp"

#include "cas/cas.hpp"
#include "cas/surrogate_cas.hpp"
#include "casxml/casxml.hpp"
#include "cas/key.hpp"
#include "cas/search_key.hpp"
#include "cas/csv_importer.hpp"

#include <iostream>
#include <chrono>


template<class VType>
benchmark::QueryExperiment<VType>::QueryExperiment(
      const std::string& dataset_filename,
      const char dataset_delim,
      const std::vector<Approach>& approaches,
      std::vector<cas::SearchKey<VType>> queries)
  : dataset_filename_(dataset_filename)
  , dataset_delim_(dataset_delim)
  , approaches_(approaches)
  , queries_(queries)
{
}


template<class VType>
void benchmark::QueryExperiment<VType>::Run() {
  std::cout << "Queries: " << std::endl;
  for (size_t i = 0; i < queries_.size(); ++i) {
    std::cout << "q" << i << ": ";
    queries_[i].DumpConcise();
  }
  std::cout << std::endl;

  int nr_repetitions = 100;
  for (const auto& approach : approaches_) {
    auto index = benchmark::CreateIndex<VType>(approach);
    PopulateIndex(*index);
    RunIndex(*index, nr_repetitions);
  }
  PrintOutput();
}


template<class VType>
void benchmark::QueryExperiment<VType>::RunIndex(
    cas::Index<VType>& index, int nr_repetitions) {
  index.Describe();
  std::cout << std::endl;

  for (auto& skey : queries_) {
    std::vector<cas::QueryStats> stats;
    stats.reserve(nr_repetitions);
    for (int i = 0; i < nr_repetitions; ++i) {
      stats.push_back(index.QueryRuntime(skey));
    }
    results_.push_back(cas::QueryStats::Avg(stats));
  }
}


template<class VType>
void benchmark::QueryExperiment<VType>::PopulateIndex(
    cas::Index<VType>& index) {
  cas::CsvImporter<VType> importer(index, dataset_delim_);
  importer.BulkLoad(dataset_filename_);
}


template<class VType>
void benchmark::QueryExperiment<VType>::PrintOutput() {
  std::cout << "query";
  for (const auto& approach : approaches_) {
    std::cout << "," << approach.name_;
  }
  std::cout << std::endl;

  for (size_t row = 0; row < queries_.size(); ++row) {
    std::cout << "q" << row;
    for (size_t col = 0; col < approaches_.size(); ++col) {
      const auto& result = results_[row + col*queries_.size()];
      double runtime_ms = result.runtime_mus_ / 1000.0;
      std::cout << "," << runtime_ms;
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;
}


// explicit instantiations to separate header from implementation
template class benchmark::QueryExperiment<cas::vint32_t>;
template class benchmark::QueryExperiment<cas::vint64_t>;
template class benchmark::QueryExperiment<cas::vstring_t>;
