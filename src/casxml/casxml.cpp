#include "casxml/casxml.hpp"
#include "cas/utils.hpp"
#include "cas/search_key.hpp"

#include <chrono>
#include <iostream>
#include <set>
#include <functional>


static std::string IRRELEVANT = "irrelevant";


template<class VType>
casxml::CasXml<VType>::~CasXml() {
}


template<class VType>
uint64_t casxml::CasXml<VType>::BulkLoad(std::deque<cas::Key<VType>>& keys) {
  const auto& t_start = std::chrono::high_resolution_clock::now();
  for (auto& key : keys) {
    Insert(key);
  }
  const auto& t_end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(t_end-t_start).count();
}


template<class VType>
void casxml::CasXml<VType>::Insert(cas::Key<VType>& key) {
  uint64_t pcr = ComputePcr(key);
  casxml::Entry entry = {
    .pcr_ = pcr,
    .ref_ = key.ref_,
  };
  btree_.insert2(key.value_, entry);
}


template<class VType>
uint64_t casxml::CasXml<VType>::ComputePcr(cas::Key<VType>& key) {
  cas::SearchKey<cas::vstring_t> dg_key;
  dg_key.low_  = IRRELEVANT;
  dg_key.high_ = IRRELEVANT;
  dg_key.path_ = { cas::Utils::JoinPath(key.path_) };

  bool found = false;
  uint64_t pcr = 0;
  data_guide_.Query(dg_key, [&](const cas::Key<cas::vstring_t>& match) -> void {
    pcr = match.ref_;
    found = true;
  });

  if (found) {
    return pcr;
  }

  ++max_pcr_;
  cas::Key<cas::vstring_t> pcrkey;
  pcrkey.value_ = IRRELEVANT;
  pcrkey.path_ = key.path_;
  pcrkey.ref_ = max_pcr_;
  data_guide_.Insert(pcrkey);
  return max_pcr_;
}


template<class VType>
void casxml::CasXml<VType>::Delete(cas::Key<VType>& key) {
  btree_iterator it;
  while ((it = btree_.find(key.value_)) != btree_.end()) {
    if (it->second.ref_ == key.ref_) {
      btree_.erase(it);
    }
  }
}


template<class VType>
const cas::QueryStats casxml::CasXml<VType>::Query(cas::SearchKey<VType>& key,
    bool decode, cas::Emitter<VType> emitter) {
  cas::QueryStats stats;
  const auto& t_start = std::chrono::high_resolution_clock::now();
  auto pcrs = QueryDataGuide(key, stats, decode);
  const auto& t_value_start = std::chrono::high_resolution_clock::now();
  auto it  = btree_.lower_bound(key.low_);
  auto end = btree_.upper_bound(key.high_);
  for (; it != end; ++it) {
    auto pcrs_it = pcrs.find(it->second.pcr_);
    if (pcrs_it != pcrs.end()) {
      ++stats.nr_matches_;
      if (decode) {
        cas::Key<VType> matched_key;
        matched_key.value_ = it->first;
        matched_key.ref_ = it->second.ref_;
        matched_key.path_ = pcrs_it->second;
        emitter(matched_key);
      }
    }
  }
  const auto& t_value_end = std::chrono::high_resolution_clock::now();
  const auto& t_end = std::chrono::high_resolution_clock::now();
  stats.value_matching_mus_ =
    std::chrono::duration_cast<std::chrono::microseconds>(t_value_end-t_value_start).count();
  stats.runtime_mus_ =
    std::chrono::duration_cast<std::chrono::microseconds>(t_end-t_start).count();
  return stats;
}


template<class VType>
const cas::QueryStats casxml::CasXml<VType>::Query(cas::SearchKey<VType>& key,
    cas::Emitter<VType> emitter) {
  return Query(key, true, emitter);
}


template<class VType>
const cas::QueryStats casxml::CasXml<VType>::QueryRuntime(
    cas::SearchKey<VType>& skey) {
  return Query(skey, false, [](const cas::Key<VType>&) -> void {});
}


template<class VType>
const typename casxml::CasXml<VType>::pcr_map
casxml::CasXml<VType>::QueryDataGuide(cas::SearchKey<VType>& key,
    cas::QueryStats& stats, bool decode) {
  const auto& t_path_start = std::chrono::high_resolution_clock::now();
  casxml::CasXml<VType>::pcr_map map;

  cas::SearchKey<cas::vstring_t> skey;
  skey.low_  = IRRELEVANT;
  skey.high_ = IRRELEVANT;
  skey.path_ = key.path_;

  cas::QueryStats dg_stats;
  if (decode) {
    dg_stats = data_guide_.Query(skey,
        [&](const cas::Key<cas::vstring_t>& match) -> void {
      map[match.ref_] = match.path_;
    });
  } else {
    dg_stats = data_guide_.Query(skey,
        [&](const std::vector<uint8_t>& /*buffer_path*/,
            const std::vector<uint8_t>& /*buffer_value*/,
            cas::ref_t pcr) -> void {
      map[pcr] = {};
    });
  }

  stats.read_path_nodes_  = dg_stats.read_path_nodes_;
  stats.read_value_nodes_ = dg_stats.read_value_nodes_;

  const auto& t_path_end = std::chrono::high_resolution_clock::now();
  stats.path_matching_mus_ =
    std::chrono::duration_cast<std::chrono::microseconds>(t_path_end-t_path_start).count();

  return map;
}


template<class VType>
void casxml::CasXml<VType>::Dump() {
  data_guide_.Dump();
  for (auto it = btree_.begin(); it != btree_.end(); ++it) {
    std::cout << "Value: " << it->first << std::endl;
    it->second.Dump();
  }
  std::cout << std::endl;
}


template<class VType>
void casxml::CasXml<VType>::Describe() const {
  std::cout << CasType() << std::endl;
  size_t btree_size = casxml::btree_memory_used.load();
  std::cout << "BTree Size: " << btree_size << std::endl;
  std::cout << "Data Guide:" << std::endl;
  data_guide_.Describe();
}


template<class VType>
const cas::IndexStats casxml::CasXml<VType>::Stats() const {
  cas::IndexStats stats = data_guide_.Stats();
  size_t btree_size = casxml::btree_memory_used.load();
  stats.size_bytes_ += btree_size;
  return stats;
}


template<class VType>
size_t casxml::CasXml<VType>::NrKeys() const {
  // TODO: store number of keys
  return 0;
}


namespace casxml {

template<>
std::string casxml::CasXml<cas::vint32_t>::CasType() const {
  return "CasXml<int32_t>";
}
template<>
std::string casxml::CasXml<cas::vint64_t>::CasType() const {
  return "CasXml<int64_t>";
}
template<>
std::string casxml::CasXml<cas::vstring_t>::CasType() const {
  return "CasXml<std::string>";
}

} // namespace casxml


// explicit instantiations to separate header from implementation
template class casxml::CasXml<cas::vint32_t>;
template class casxml::CasXml<cas::vint64_t>;
template class casxml::CasXml<cas::vstring_t>;
