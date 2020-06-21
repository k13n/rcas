#include "cas/interleaver.hpp"
#include "cas/key_encoding.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>


cas::InterleavedKey cas::Interleaver::Interleave(const cas::BinaryKey& bkey,
    const cas::IndexType& type) {
  switch (type) {
  case cas::IndexType::PathValue:
    return cas::Interleaver::PathValue(bkey);
  case cas::IndexType::ValuePath:
    return cas::Interleaver::ValuePath(bkey);
  case cas::IndexType::LabelWise:
    return cas::Interleaver::LabelWise(bkey);
  case cas::IndexType::ZOrder:
    return cas::Interleaver::ZOrder(bkey);
  default:
    assert(false);
    exit(-1);
  }
}


cas::InterleavedKey cas::Interleaver::ZOrder(const cas::BinaryKey& bkey) {
  cas::InterleavedKey ikey;
  ikey.bytes_.reserve(bkey.path_.size() + bkey.value_.size());
  ikey.ref_ = bkey.ref_;

  int p_run_len = (int) std::ceil(bkey.path_.size()  / ((double) bkey.value_.size()));
  int v_run_len = (int) std::ceil(bkey.value_.size() / ((double) bkey.path_.size()));
  size_t p_pos = 0;
  size_t v_pos = 0;

  while (p_pos < bkey.path_.size() || v_pos < bkey.value_.size()) {
    for (int i = 0; i < v_run_len && v_pos < bkey.value_.size(); ++i) {
      ikey.bytes_.push_back({ .byte_ = bkey.value_[v_pos], .dimension_ = Dimension::Value });
      ++v_pos;
    }
    for (int i = 0; i < p_run_len && p_pos < bkey.path_.size(); ++i) {
      ikey.bytes_.push_back({ .byte_ = bkey.path_[p_pos],  .dimension_ = Dimension::Path });
      ++p_pos;
    }
  }

  return ikey;
}


cas::InterleavedKey cas::Interleaver::LabelWise(const cas::BinaryKey& bkey) {
  size_t nr_bytes = bkey.path_.size() + bkey.value_.size();

  cas::InterleavedKey ikey;
  ikey.bytes_.reserve(nr_bytes);
  ikey.ref_ = bkey.ref_;

  size_t p_pos = 0;
  size_t v_pos = 0;

  auto take_path_label = [&]() -> void {
    if (p_pos >= bkey.path_.size()) {
      return;
    }
    do {
      ikey.bytes_.push_back({ .byte_ = bkey.path_[p_pos], .dimension_ = Dimension::Path });
      ++p_pos;
    } while (p_pos < bkey.path_.size() && bkey.path_[p_pos] != cas::kPathSep);
    if (p_pos < bkey.path_.size()) {
      ikey.bytes_.push_back({ .byte_ = bkey.path_[p_pos], .dimension_ = Dimension::Path });
      ++p_pos;
    }
  };
  auto take_value_byte = [&]() -> void {
    if (v_pos >= bkey.value_.size()) {
      return;
    }
    ikey.bytes_.push_back({ .byte_ = bkey.value_[v_pos], .dimension_ = Dimension::Value });
    ++v_pos;
  };

  while (v_pos + p_pos < nr_bytes) {
    take_path_label();
    take_value_byte();
  }

  return ikey;
}


cas::InterleavedKey cas::Interleaver::PathValue(const cas::BinaryKey& bkey) {
  cas::InterleavedKey ikey;
  ikey.bytes_.reserve(bkey.path_.size() + bkey.value_.size());
  ikey.ref_ = bkey.ref_;
  for (size_t i = 0; i < bkey.path_.size(); ++i) {
    ikey.bytes_.push_back({ .byte_ = bkey.path_[i], .dimension_ = Dimension::Path });
  }
  for (size_t i = 0; i < bkey.value_.size(); ++i) {
    ikey.bytes_.push_back({ .byte_ = bkey.value_[i], .dimension_ = Dimension::Value });
  }
  return ikey;
}


cas::InterleavedKey cas::Interleaver::ValuePath(const cas::BinaryKey& bkey) {
  cas::InterleavedKey ikey;
  ikey.bytes_.reserve(bkey.path_.size() + bkey.value_.size());
  ikey.ref_ = bkey.ref_;
  for (size_t i = 0; i < bkey.value_.size(); ++i) {
    ikey.bytes_.push_back({ .byte_ = bkey.value_[i], .dimension_ = Dimension::Value });
  }
  for (size_t i = 0; i < bkey.path_.size(); ++i) {
    ikey.bytes_.push_back({ .byte_ = bkey.path_[i], .dimension_ = Dimension::Path });
  }
  return ikey;
}
