#include "benchmark/space_experiment.hpp"
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
benchmark::SpaceExperiment<VType>::SpaceExperiment(
      const std::string& dataset_filename,
      const char dataset_delim,
      const std::vector<Approach>& approaches)
  : dataset_filename_(dataset_filename)
  , dataset_delim_(dataset_delim)
  , approaches_(approaches)
{
}


template<class VType>
void benchmark::SpaceExperiment<VType>::Run() {
  std::cout << "dataset: " << dataset_filename_ << std::endl;
  std::cout << std::endl;

  for (const auto& approach : approaches_) {
    auto index = benchmark::CreateIndex<VType>(approach);
    PopulateIndex(*index);
    RunIndex(*index);
  }
  PrintOutput();
}


template<class VType>
void benchmark::SpaceExperiment<VType>::RunIndex(
    cas::Index<VType>& index) {
  index.Describe();
  results_.push_back(index.Stats());
}


template<class VType>
void benchmark::SpaceExperiment<VType>::PopulateIndex(
    cas::Index<VType>& index) {
  cas::CsvImporter<VType> importer(index, dataset_delim_);
  importer.BulkLoad(dataset_filename_);
}


template<class VType>
void benchmark::SpaceExperiment<VType>::PrintOutput() {
  std::cout << "dataset";
  for (const auto& approach : approaches_) {
    std::cout << "," << approach.name_;
  }
  std::cout << std::endl;
  std::cout << "x";
  for (size_t i = 0; i < approaches_.size(); ++i) {
    const auto& result = results_[i];
    std::cout << "," << result.size_bytes_;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "dataset";
  for (const auto& approach : approaches_) {
    std::cout << "," << approach.name_;
  }
  std::cout << std::endl;
  std::cout << "x";
  for (size_t i = 0; i < approaches_.size(); ++i) {
    const auto& result = results_[i];
    std::cout << "," << (result.size_bytes_ / static_cast<double>(result.nr_keys_));
  }
  std::cout << std::endl;
  std::cout << std::endl;
}


// explicit instantiations to separate header from implementation
template class benchmark::SpaceExperiment<cas::vint32_t>;
template class benchmark::SpaceExperiment<cas::vint64_t>;
template class benchmark::SpaceExperiment<cas::vstring_t>;
