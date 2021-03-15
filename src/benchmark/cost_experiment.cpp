#include "benchmark/cost_experiment.hpp"
#include "benchmark/selectivity_computer.hpp"
#include "benchmark/utils.hpp"

#include "cas/cas.hpp"
#include "cas/surrogate_cas.hpp"
/* #include "casxml/casxml.hpp" */
#include "cas/key.hpp"
#include "cas/search_key.hpp"
#include "cas/csv_importer.hpp"

#include <iostream>
#include <chrono>


template<class VType>
benchmark::CostExperiment<VType>::CostExperiment(
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
void benchmark::CostExperiment<VType>::Run() {
  std::cout << "filename: " << dataset_filename_ << std::endl << std::endl;
  std::cout << "Queries:" << std::endl;
  for (size_t i = 0; i < queries_.size(); ++i) {
    std::cout << "q" << i << ": ";
    queries_[i].DumpConcise();
  }
  std::cout << std::endl;

  /* int nr_repetitions = 100; */
  int nr_repetitions = 1;
  for (const auto& approach : approaches_) {
    auto index = benchmark::CreateIndex<VType>(approach);
    PopulateIndex(*index);
    RunIndex(*index, nr_repetitions);
    ComputeSelectivities(*index);
  }
  PrintOutput();
}


template<class VType>
void benchmark::CostExperiment<VType>::RunIndex(
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
void benchmark::CostExperiment<VType>::ComputeSelectivities(
    cas::Index<VType>& index) {
  for (auto& skey : queries_) {
    benchmark::SelectivityComputer<VType> s(index);
    s.Compute(skey);
    std::cout << std::endl;
    std::cout << std::endl;
  }
}


template<class VType>
void benchmark::CostExperiment<VType>::PopulateIndex(
    cas::Index<VType>& index) {
  cas::CsvImporter<VType> importer(index, dataset_delim_);
  importer.BulkLoad(dataset_filename_);
}


template<class VType>
void benchmark::CostExperiment<VType>::PrintOutput() {
  std::cout << "runtime measurements:" << std::endl;
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

  std::cout << "traversed nodes" << std::endl;
  std::cout << "query";
  for (const auto& approach : approaches_) {
    std::cout << "," << approach.name_;
  }
  std::cout << std::endl;

  for (size_t row = 0; row < queries_.size(); ++row) {
    std::cout << "q" << row;
    for (size_t col = 0; col < approaches_.size(); ++col) {
      const auto& result = results_[row + col*queries_.size()];
      int64_t read_nodes = result.read_path_nodes_
        + result.read_value_nodes_
        + result.read_leaf_nodes_;
      std::cout << "," << read_nodes;
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

}


// explicit instantiations to separate header from implementation
template class benchmark::CostExperiment<cas::vint32_t>;
template class benchmark::CostExperiment<cas::vint64_t>;
template class benchmark::CostExperiment<cas::vstring_t>;
