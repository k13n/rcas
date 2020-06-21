#include "cas/cas.hpp"
#include "cas/key_encoder.hpp"
#include "cas/node0.hpp"
#include "cas/prefix_matcher.hpp"
#include "cas/insertion.hpp"
#include "cas/insertion_static.hpp"
#include "cas/interleaved_key.hpp"
#include "cas/interleaver.hpp"
#include "cas/query.hpp"
#include "cas/search_key.hpp"
#include "cas/key_decoder.hpp"
#include "cas/utils.hpp"
#include "cas/bulk_load.hpp"
#include <iostream>
#include <cassert>
#include <chrono>


template<class VType>
cas::Cas<VType>::Cas(cas::IndexType type)
  : index_type_(type)
{ }


template<class VType>
cas::Cas<VType>::~Cas() {
  DeleteNodesRecursively(root_);
}


template<class VType>
void cas::Cas<VType>::DeleteNodesRecursively(cas::Node *node) {
  if (node == nullptr) {
    return;
  }
  node->ForEachChild([&](uint8_t /* byte  */, cas::Node& child) -> bool {
    DeleteNodesRecursively(&child);
    return true;
  });
  delete node;
}


template<class VType>
void cas::Cas<VType>::Insert(cas::Key<VType>& key) {
  cas::KeyEncoder<VType> encoder;
  BinaryKey bkey = encoder.Encode(key);
  Insert(bkey);
}


template<class VType>
void cas::Cas<VType>::Insert(cas::BinaryKey& bkey) {
  switch (index_type_) {
  case cas::IndexType::DynamicInterleaving:
    {
      cas::Insertion insertion(&root_, bkey);
      if (insertion.Execute()) {
        ++nr_keys_;
      }
      break;
    }
  case cas::IndexType::PathValue:
  case cas::IndexType::ValuePath:
  case cas::IndexType::LabelWise:
  case cas::IndexType::ZOrder:
    {
      cas::InterleavedKey ikey = cas::Interleaver::PathValue(bkey);
      cas::InsertionStatic insertion(&root_, ikey);
      if (insertion.Execute()) {
        ++nr_keys_;
      }
      break;
    }
  default:
    assert(false); // NOLINT
    exit(-1);
  }
}


template<class VType>
uint64_t cas::Cas<VType>::BulkLoad(std::deque<cas::Key<VType>>& keys) {
  cas::KeyEncoder<VType> encoder;
  std::deque<cas::BinaryKey> bkeys;
  for (const auto& key : keys) {
    bkeys.push_back(encoder.Encode(key));
  }
  // releasing memory for old keys
  keys.clear();
  keys.shrink_to_fit();
  return BulkLoad(bkeys);
}


template<class VType>
uint64_t cas::Cas<VType>::BulkLoad(std::deque<cas::BinaryKey>& keys) {
  const auto& t_start = std::chrono::high_resolution_clock::now();
  if (index_type_ == cas::IndexType::DynamicInterleaving) {
    /* cas::BulkLoad load(keys, cas::Dimension::Path); */
    cas::BulkLoad load(keys, cas::Dimension::Value);
    root_ = load.Execute();
    nr_keys_ = keys.size();
  } else {
    for (auto& bkey : keys) {
      cas::InterleavedKey ikey = cas::Interleaver::Interleave(bkey, index_type_);
      cas::InsertionStatic insertion(&root_, ikey);
      if (insertion.Execute()) {
        ++nr_keys_;
      }
    }
  }

  const auto& t_end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(t_end-t_start).count();
}




template<class VType>
cas::QueryStats cas::Cas<VType>::Query(
    cas::SearchKey<VType>& key,
    const cas::BinaryKeyEmitter& emitter) {
  cas::KeyEncoder<VType> encoder;
  cas::BinarySK bkey = encoder.Encode(key);
  cas::PathMatcher pm;
  cas::Query<VType> query(root_, bkey, pm, emitter);
  query.Execute();
  return query.Stats();
}


template<class VType>
cas::QueryStats cas::Cas<VType>::Query(
    cas::SearchKey<VType>& key,
    const cas::RefEmitter& emitter) {
  return Query(key, [&](
        const std::vector<uint8_t>& /*buffer_path*/,
        const std::vector<uint8_t>& /*buffer_value*/,
        cas::ref_t ref) -> void {
    emitter(ref);
  });
}


template<class VType>
cas::QueryStats cas::Cas<VType>::Query(
    cas::SearchKey<VType>& key,
    const cas::Emitter<VType>& emitter) {
  cas::KeyDecoder<VType> decoder;
  return Query(key, [&](
        const std::vector<uint8_t>& buffer_path,
        const std::vector<uint8_t>& buffer_value,
        cas::ref_t ref) -> void {
    emitter(decoder.Decode(buffer_path, buffer_value, ref));
  });
}


template<class VType>
cas::QueryStats cas::Cas<VType>::QueryRuntime(
    cas::SearchKey<VType>& key) {
  return Query(key, [&](
        const std::vector<uint8_t>& /*buffer_path*/,
        const std::vector<uint8_t>& /*buffer_value*/,
        cas::ref_t /*ref*/) -> void {
  });
}


template<class VType>
size_t cas::Cas<VType>::Size() const {
  return nr_keys_;
}


template<class VType>
cas::IndexStats cas::Cas<VType>::Stats() const {
  cas::IndexStats stats;
  if (root_ != nullptr) {
    root_->CollectStats(stats, 0);
  }
  stats.nr_keys_ = nr_keys_;
  return stats;
}


template<class VType>
void cas::Cas<VType>::Describe() const {
  switch (index_type_) {
  case cas::IndexType::DynamicInterleaving:
    std::cout << "Cas";
    break;
  case cas::IndexType::PathValue:
    std::cout << "CasPV";
    break;
  case cas::IndexType::ValuePath:
    std::cout << "CasVP";
    break;
  case cas::IndexType::LabelWise:
    std::cout << "CasLI";
    break;
  case cas::IndexType::ZOrder:
    std::cout << "CasZO";
    break;
  case cas::IndexType::Xml:
    assert(false); // NOLINT
  }
  std::cout << "<" << cas::Utils::TypeToString<VType>() << ">" << std::endl;
  std::cout << "Size (keys):  " << nr_keys_ << std::endl;
  cas::IndexStats stats;
  if (root_ != nullptr) {
    root_->CollectStats(stats, 0);
  }
  std::cout << "Size (bytes): " << stats.size_bytes_ << std::endl;
  double bpk = nr_keys_ == 0 ? 0 :
      stats.size_bytes_ / static_cast<double>(nr_keys_);
  size_t internal_nodes = stats.nr_nodes_ - stats.nr_node0_;
  std::cout << "Bytes/Key:    " << bpk << std::endl;
  std::cout << "Total Nodes:  " << stats.nr_nodes_ << std::endl;
  std::cout << "Inner Nodes:  " << internal_nodes << std::endl;
  std::cout << "Path Nodes:   " << stats.nr_path_nodes_ << std::endl;
  std::cout << "Value Nodes:  " << stats.nr_value_nodes_ << std::endl;
  std::cout << "Node0:        " << stats.nr_node0_ << std::endl;
  std::cout << "P-Node4:      " << stats.nr_p_node4_ << std::endl;
  std::cout << "P-Node16:     " << stats.nr_p_node16_ << std::endl;
  std::cout << "P-Node48:     " << stats.nr_p_node48_ << std::endl;
  std::cout << "P-Node256:    " << stats.nr_p_node256_ << std::endl;
  std::cout << "V-Node4:      " << stats.nr_v_node4_ << std::endl;
  std::cout << "V-Node16:     " << stats.nr_v_node16_ << std::endl;
  std::cout << "V-Node48:     " << stats.nr_v_node48_ << std::endl;
  std::cout << "V-Node256:    " << stats.nr_v_node256_ << std::endl;
  std::cout << "PP Steps:     " << stats.pp_steps << std::endl;
  std::cout << "VV Steps:     " << stats.vv_steps << std::endl;
  std::cout << "PV Steps:     " << stats.pv_steps << std::endl;
  std::cout << "VP Steps:     " << stats.vp_steps << std::endl;
  std::cout << "Max Depth:    " << stats.max_depth_ << std::endl;

  double avg_fanout = 0;
  if (internal_nodes > 0) {
    avg_fanout = stats.nr_nodes_ / static_cast<double>(internal_nodes);
  }
  std::cout << "Avg Fanout:   " << avg_fanout << std::endl;

  size_t step_diff = stats.pv_steps + stats.vp_steps;
  size_t step_all  = stats.pp_steps + stats.vv_steps + step_diff;
  double il_ratio = 0;
  if (step_all > 0) {
    il_ratio = step_diff / static_cast<double>(step_all);
  }
  std::cout << "IL Ratio:     " << il_ratio << std::endl;

  std::cout << std::endl;
}


template<class VType>
void cas::Cas<VType>::Dump() const {
  if (root_ != nullptr) {
    root_->DumpRecursive();
  }
}


template<class VType>
void cas::Cas<VType>::DumpConcise() const {
  if (root_ != nullptr) {
    root_->DumpConcise(0x00, 0);
  }
}


template<class VType>
size_t cas::Cas<VType>::NrKeys() const {
  return nr_keys_;
}


// explicit instantiations to separate header from implementation
template class cas::Cas<cas::vint32_t>;
template class cas::Cas<cas::vint64_t>;
template class cas::Cas<cas::vstring_t>;
