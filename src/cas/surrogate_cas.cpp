#include "cas/surrogate_cas.hpp"
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
cas::SurrogateCas<VType>::SurrogateCas(cas::IndexType type,
    size_t s_max_depth, size_t s_bytes_per_label)
  : index_(type)
  , surrogate_(s_max_depth, s_bytes_per_label)
{ }


template<class VType>
void cas::SurrogateCas<VType>::Insert(cas::Key<VType>& key) {
  cas::KeyEncoder<VType> encoder;
  BinaryKey bkey = encoder.Encode(key, surrogate_);
  index_.Insert(bkey);
}


template<class VType>
uint64_t cas::SurrogateCas<VType>::BulkLoad(
    std::deque<cas::Key<VType>>& keys) {
  cas::KeyEncoder<VType> encoder;
  std::deque<cas::BinaryKey> bkeys;
  for (const auto& key : keys) {
    bkeys.push_back(encoder.Encode(key, surrogate_));
  }
  // releasing memory for old keys
  keys.clear();
  keys.shrink_to_fit();
  return index_.BulkLoad(bkeys);
}


template<class VType>
cas::QueryStats cas::SurrogateCas<VType>::Query(
    cas::SearchKey<VType>& key,
    const cas::BinaryKeyEmitter& emitter) {
  cas::KeyEncoder<VType> encoder;
  cas::BinarySK bkey = encoder.Encode(key, surrogate_);
  cas::SurrogatePathMatcher pm(surrogate_);
  cas::Query<VType> query(index_.root_, bkey, pm, emitter);
  query.Execute();
  return query.Stats();
}


template<class VType>
cas::QueryStats cas::SurrogateCas<VType>::Query(
    cas::SearchKey<VType>& key,
    const cas::Emitter<VType>& emitter) {
  cas::KeyDecoder<VType> decoder;
  return Query(key, [&](
        const std::vector<uint8_t>& buffer_path,
        const std::vector<uint8_t>& buffer_value,
        cas::ref_t ref) -> void {
    emitter(decoder.Decode(surrogate_, buffer_path, buffer_value, ref));
  });
}


template<class VType>
cas::QueryStats cas::SurrogateCas<VType>::QueryRuntime(
    cas::SearchKey<VType>& key) {
  return Query(key, [&](
        const std::vector<uint8_t>& /*buffer_path*/,
        const std::vector<uint8_t>& /*buffer_value*/,
        cas::ref_t /*ref*/) -> void {
  });
}


template<class VType>
cas::IndexStats cas::SurrogateCas<VType>::Stats() const {
  return index_.Stats();
}


template<class VType>
void cas::SurrogateCas<VType>::Describe() const {
  index_.Describe();
  std::cout << "S. MaxDepth:  " << surrogate_.max_depth_ << std::endl;
  std::cout << "S. BytesPerLabel: " << surrogate_.bytes_per_label_ << std::endl;
}


template<class VType>
size_t cas::SurrogateCas<VType>::NrKeys() const {
  return index_.NrKeys();
}


// explicit instantiations to separate header from implementation
template class cas::SurrogateCas<cas::vint32_t>;
template class cas::SurrogateCas<cas::vint64_t>;
template class cas::SurrogateCas<cas::vstring_t>;
