#include "benchmark/structure_experiment.hpp"
#include "benchmark/utils.hpp"

#include "cas/cas.hpp"
/* #include "casxml/casxml.hpp" */
#include "cas/key.hpp"
#include "cas/search_key.hpp"
#include "cas/csv_importer.hpp"

#include <iostream>
#include <chrono>


template<class VType>
benchmark::StructureExperiment<VType>::StructureExperiment(
      const std::string& dataset,
      const char dataset_delim)
  : dataset_(dataset)
  , dataset_delim_(dataset_delim)
{
}


template<class VType>
void benchmark::StructureExperiment<VType>::Run() {
  cas::Cas<VType> index(cas::IndexType::DynamicInterleaving);
  PopulateIndex(index);

  index.Describe();
  PrintOutput(index.Stats());
}


template<class VType>
void benchmark::StructureExperiment<VType>::PopulateIndex(
    cas::Index<VType>& index) {
  cas::CsvImporter<VType> importer(index, dataset_delim_);
  importer.BulkLoad(dataset_);
}


template<class VType>
void benchmark::StructureExperiment<VType>::PrintOutput(
    const cas::IndexStats& stats) {
  std::cout << std::endl;

  // block #1: horizontal structure
  std::cout << "node_type,nr_nodes,nr_l_nodes,nr_p_nodes,nr_v_nodes" << std::endl;
  std::cout << "0"
    << "," << stats.nr_node0_
    << "," << stats.nr_node0_
    << ",0"
    << ",0" << std::endl;
  std::cout << "4"
    << "," << (stats.nr_p_node4_ + stats.nr_v_node4_)
    << ",0"
    << "," << stats.nr_p_node4_
    << "," << stats.nr_v_node4_
    << std::endl;
  std::cout << "16"
    << "," << (stats.nr_p_node16_ + stats.nr_v_node16_)
    << ",0"
    << "," << stats.nr_p_node16_
    << "," << stats.nr_v_node16_
    << std::endl;
  std::cout << "48"
    << "," << (stats.nr_p_node48_ + stats.nr_v_node48_)
    << ",0"
    << "," << stats.nr_p_node48_
    << "," << stats.nr_v_node48_
    << std::endl;
  std::cout << "256"
    << "," << (stats.nr_p_node256_ + stats.nr_v_node256_)
    << ",0"
    << "," << stats.nr_p_node256_
    << "," << stats.nr_v_node256_
    << std::endl;
  std::cout << std::endl;

  // block #1: vertical structure
  std::cout << "depth,nr_nodes" << std::endl;
  for (size_t depth = 0; depth <= stats.max_depth_; ++depth) {
    size_t nr_nodes = stats.depth_histo_.at(depth);
    std::cout << depth << "," << nr_nodes  << std::endl;
  }
  std::cout << std::endl;
}


// explicit instantiations to separate header from implementation
template class benchmark::StructureExperiment<cas::vint32_t>;
template class benchmark::StructureExperiment<cas::vint64_t>;
template class benchmark::StructureExperiment<cas::vstring_t>;
