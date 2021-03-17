#include "benchmark/scalability_experiment2.hpp"
#include "benchmark/utils.hpp"

#include "cas/cas.hpp"
#include "cas/surrogate_cas.hpp"
/* #include "casxml/casxml.hpp" */
#include "cas/key.hpp"
#include "cas/search_key.hpp"
#include "cas/partition_importer.hpp"

#include <iostream>
#include <chrono>


benchmark::ScalabilityExperiment2::ScalabilityExperiment2(
      const std::vector<Dataset>& datasets)
  : datasets_(datasets)
{ }


void benchmark::ScalabilityExperiment2::Run() {
  for (const auto& dataset : datasets_) {
    std::cout
      << "dataset: " << dataset.filename_
      << " " << dataset.dataset_size_ << std::endl;
    cas::Cas<cas::vint64_t> index(cas::IndexType::DynamicInterleaving);
    RunIndex(index, dataset);
  }
  PrintOutput();
}


void benchmark::ScalabilityExperiment2::RunIndex(
    cas::Cas<cas::vint64_t>& index, const Dataset& dataset) {
  uint64_t load_time = PopulateIndex(index, dataset);
  index.Describe();
  std::cout << "runtime_mus_: " << load_time << "\n";
  std::cout << "runtime_ms_: " << (load_time / 1000.0) << "\n";
  std::cout << "runtime_s_: " << (load_time /  1'000'000.0) << "\n";
  std::cout << "runtime_m_: " << (load_time / 60'000'000.0) << "\n";
  std::cout << "runtime_h_: " << (load_time / (60*60'000'000.0)) << "\n";
  std::cout << std::endl;
  cas::IndexStats stats = index.Stats();
  results_.push_back(stats);
  load_times_.push_back(load_time);
}


uint64_t benchmark::ScalabilityExperiment2::PopulateIndex(
    cas::Cas<cas::vint64_t>& index,
    const Dataset& dataset) const {
  cas::PartitionImporter<cas::vint64_t> importer(index);
  return importer.BulkLoad(
      dataset.filename_,
      dataset.dataset_size_,
      dataset.page_size_);
}


void benchmark::ScalabilityExperiment2::PrintOutput() {
  std::cout << "size_b_;runtime_mus_\n";
  for (size_t i = 0; i < datasets_.size(); ++i) {
    const auto& dataset = datasets_[i];
    std::cout << dataset.dataset_size_ << ";";
    std::cout << load_times_[i] << "\n";
  }
  std::cout << "\n\n" << std::flush;
}
