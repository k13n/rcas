#include "benchmark/scalability_experiment.hpp"
#include "benchmark/utils.hpp"

#include "cas/cas.hpp"
#include "cas/surrogate_cas.hpp"
#include "casxml/casxml.hpp"
#include "cas/key.hpp"
#include "cas/search_key.hpp"
#include "cas/csv_importer.hpp"

#include <iostream>
#include <chrono>


benchmark::ScalabilityExperiment::ScalabilityExperiment(
      const std::vector<Dataset>& datasets,
      const char dataset_delim,
      const std::vector<Approach>& approaches)
  : datasets_(datasets)
  , dataset_delim_(dataset_delim)
  , approaches_(approaches)
{ }


void benchmark::ScalabilityExperiment::Run() {
  for (const auto& dataset : datasets_) {
    std::cout << "dataset: " << dataset.filename_ << std::endl;
    for (const auto& approach : approaches_) {
      cas::Index<cas::vint32_t>* index = benchmark::CreateIndex<cas::vint32_t>(approach);
      RunIndex(*index, dataset);
      delete index;
    }
  }
  PrintOutput();
}


void benchmark::ScalabilityExperiment::RunIndex(
    cas::Index<cas::vint32_t>& index, const Dataset& dataset) {
  uint64_t load_time = PopulateIndex(index, dataset.filename_);
  index.Describe();
  std::cout << std::endl;
  cas::IndexStats stats = index.Stats();
  results_.push_back(stats);
  load_times_.push_back(load_time);
}


uint64_t benchmark::ScalabilityExperiment::PopulateIndex(
    cas::Index<cas::vint32_t>& index, const std::string& filename) const {
  cas::CsvImporter<cas::vint32_t> importer(index, dataset_delim_);
  return importer.BulkLoad(filename);
}


void benchmark::ScalabilityExperiment::PrintOutput() {
  std::cout << "Space Consumption:" << std::endl << std::endl;
  PrintTableSpace();
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "Bytes per key:" << std::endl << std::endl;
  PrintTableSpacePerKey();
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "Bulk Loading Time:" << std::endl << std::endl;
  PrintTableTime();
  std::cout << std::endl;
}


void benchmark::ScalabilityExperiment::PrintTableSpace() {
  std::cout << "size";
  for (const auto& approach :  approaches_) {
    std::cout << "," << approach.name_;
  }
  std::cout << std::endl;
  for (size_t row = 0; row < datasets_.size(); ++row) {
    std::cout << datasets_[row].size_;
    for (size_t col = 0; col < approaches_.size(); ++col) {
      size_t pos = row * approaches_.size() + col;
      std::cout << "," << results_[pos].size_bytes_;
    }
    std::cout << std::endl;
  }
}


void benchmark::ScalabilityExperiment::PrintTableSpacePerKey() {
  std::cout << "size";
  for (const auto& approach :  approaches_) {
    std::cout << "," << approach.name_;
  }
  std::cout << std::endl;
  for (size_t row = 0; row < datasets_.size(); ++row) {
    std::cout << datasets_[row].size_;
    for (size_t col = 0; col < approaches_.size(); ++col) {
      size_t pos = row * approaches_.size() + col;
      size_t index_size = results_[pos].size_bytes_;
      double bytes_per_key = index_size / static_cast<double>(datasets_[row].size_);
      std::cout << "," << bytes_per_key;
    }
    std::cout << std::endl;
  }
}


void benchmark::ScalabilityExperiment::PrintTableTime() {
  std::cout << "size";
  for (const auto& approach :  approaches_) {
    std::cout << "," << approach.name_;
  }
  std::cout << std::endl;
  for (size_t row = 0; row < datasets_.size(); ++row) {
    std::cout << datasets_[row].size_;
    for (size_t col = 0; col < approaches_.size(); ++col) {
      size_t pos = row * approaches_.size() + col;
      double runtime_ms = load_times_[pos] / 1000.0;
      std::cout << "," << runtime_ms;
    }
    std::cout << std::endl;
  }
}
