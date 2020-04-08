#ifndef CAS_QUERY_STATS_H_
#define CAS_QUERY_STATS_H_

#include <cstddef>
#include <cstdint>
#include <vector>


namespace cas {

struct QueryStats {
  int32_t nr_matches_ = 0;
  int32_t read_path_nodes_ = 0;
  int32_t read_value_nodes_ = 0;
  int32_t read_leaf_nodes_ = 0;
  int64_t runtime_mus_ = 0;
  int64_t value_matching_mus_ = 0;
  int64_t path_matching_mus_ = 0;
  int64_t ref_matching_mus_ = 0;

  void Dump() const;

  static QueryStats Avg(const std::vector<QueryStats>& stats);
};

} // namespace cas

#endif // CAS_QUERY_STATS_H_
