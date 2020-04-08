#ifndef CAS_INDEX_STATS_H_
#define CAS_INDEX_STATS_H_

#include <cstddef>
#include <cstdint>
#include <map>

namespace cas {


struct IndexStats {
  size_t nr_keys_ = 0;
  size_t nr_nodes_ = 0;
  size_t nr_path_nodes_ = 0;
  size_t nr_value_nodes_ = 0;
  size_t nr_node0_ = 0;
  size_t nr_p_node4_ = 0;
  size_t nr_p_node16_ = 0;
  size_t nr_p_node48_ = 0;
  size_t nr_p_node256_ = 0;
  size_t nr_v_node4_ = 0;
  size_t nr_v_node16_ = 0;
  size_t nr_v_node48_ = 0;
  size_t nr_v_node256_ = 0;
  size_t size_bytes_ = 0;
  size_t pv_steps = 0;
  size_t pp_steps = 0;
  size_t vv_steps = 0;
  size_t vp_steps = 0;
  size_t max_depth_ = 0;
  std::map<size_t,size_t> depth_histo_;
};


} // namespace cas

#endif // CAS_INDEX_STATS_H_
