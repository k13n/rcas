#include "cas/query_stats.hpp"
#include <iostream>


void cas::QueryStats::Dump() const {
  std::cout << "QueryStats" << std::endl;
  std::cout << "Matches: " << nr_matches_ << std::endl;
  std::cout << "Read Path Nodes: " << read_path_nodes_ << std::endl;
  std::cout << "Read Value Nodes: " << read_value_nodes_ << std::endl;
  std::cout << "Read Leaf Nodes: " << read_leaf_nodes_ << std::endl;
  std::cout << "Runtime (mus): " << runtime_mus_ << std::endl;
  std::cout << "Value Matching Runtime (mus): " << value_matching_mus_ << std::endl;
  std::cout << "Path Matching Runtime (mus):  " << path_matching_mus_ << std::endl;
  std::cout << "Ref Matching Runtime (mus):   " << ref_matching_mus_ << std::endl;
  std::cout << std::endl;
}


cas::QueryStats cas::QueryStats::Avg(const std::vector<cas::QueryStats>& stats) {
  cas::QueryStats result;
  result.nr_matches_ = 0;
  result.read_path_nodes_ = 0;
  result.read_value_nodes_ = 0;
  result.read_leaf_nodes_ = 0;
  result.runtime_mus_ = 0;
  result.value_matching_mus_ = 0;
  result.path_matching_mus_ = 0;
  result.ref_matching_mus_ = 0;
  if (!stats.empty()) {
    for (const auto& stat : stats) {
      result.nr_matches_ += stat.nr_matches_;
      result.read_path_nodes_ += stat.read_path_nodes_;
      result.read_value_nodes_ += stat.read_value_nodes_;
      result.read_leaf_nodes_ += stat.read_leaf_nodes_;
      result.runtime_mus_ += stat.runtime_mus_;
      result.value_matching_mus_ += stat.value_matching_mus_;
      result.path_matching_mus_ += stat.path_matching_mus_;
      result.ref_matching_mus_ += stat.ref_matching_mus_;
    }
    result.nr_matches_  = static_cast<int32_t>(result.nr_matches_  / stats.size());
    result.read_path_nodes_ = static_cast<int32_t>(result.read_path_nodes_  / stats.size());
    result.read_value_nodes_ = static_cast<int32_t>(result.read_value_nodes_  / stats.size());
    result.read_leaf_nodes_ = static_cast<int32_t>(result.read_leaf_nodes_  / stats.size());
    result.runtime_mus_ = static_cast<int64_t>(result.runtime_mus_ / stats.size());
    result.value_matching_mus_ = static_cast<int64_t>(result.value_matching_mus_ / stats.size());
    result.path_matching_mus_ = static_cast<int64_t>(result.path_matching_mus_ / stats.size());
    result.ref_matching_mus_ = static_cast<int64_t>(result.ref_matching_mus_ / stats.size());
  }
  return result;
}
