#include "cas/query.hpp"
#include "cas/node0.hpp"
#include "cas/utils.hpp"
#include <cassert>
#include <iostream>
#include <functional>
#include <chrono>


template<class VType>
cas::Query<VType>::Query(cas::Node* root,
        cas::BinarySK& key,
        cas::PathMatcher& pm,
        const cas::BinaryKeyEmitter& emitter)
    : root_(root)
    , key_(key)
    , pm_(pm)
    , emitter_(emitter)
    , buf_pat_(cas::kMaxPathLength+1, 0x00)
    , buf_val_(cas::kMaxValueLength+1, 0x00)
{}


template<class VType>
void cas::Query<VType>::Execute() {
  if (root_ == nullptr) {
    return;
  }

  const auto& t_start = std::chrono::high_resolution_clock::now();

  State initial_state;
  initial_state.node_ = root_;
  initial_state.parent_dimension_ = cas::Dimension::Path; // doesn't matter
  initial_state.parent_byte_ = 0x00; // doesn't matte;
  initial_state.len_pat_ = 0;
  initial_state.len_val_ = 0;
  initial_state.vl_pos_ = 0;
  initial_state.vh_pos_ = 0;
  stack_.push_back(initial_state);

  while (!stack_.empty()) {
    State s = stack_.back();
    stack_.pop_back();

    UpdateStats(s);
    PrepareBuffer(s);
    cas::PathMatcher::PrefixMatch match_pat = MatchPathPrefix(s);
    cas::PathMatcher::PrefixMatch match_val = MatchValuePrefix(s);

    if (match_pat == PathMatcher::PrefixMatch::MATCH &&
        match_val == PathMatcher::PrefixMatch::MATCH) {
      assert(s.node_->IsLeaf());
      EmitMatch(s);
    } else if (match_pat != PathMatcher::PrefixMatch::MISMATCH &&
               match_val != PathMatcher::PrefixMatch::MISMATCH) {
      assert(!s.node_->IsLeaf());
      Descend(s);
    }
  }

  const auto& t_end = std::chrono::high_resolution_clock::now();
  stats_.runtime_mus_ =
    std::chrono::duration_cast<std::chrono::microseconds>(t_end-t_start).count();
}


template<class VType>
void cas::Query<VType>::UpdateStats(State& s) {
  switch (s.node_->dimension_) {
  case cas::Dimension::Path:
    ++stats_.read_path_nodes_;
    break;
  case cas::Dimension::Value:
    ++stats_.read_value_nodes_;
    break;
  case cas::Dimension::Leaf:
    ++stats_.read_leaf_nodes_;
    break;
  }
}


template<class VType>
void cas::Query<VType>::PrepareBuffer(State& s) {
  if (s.node_ != root_) {
    switch (s.parent_dimension_) {
    case cas::Dimension::Path:
      buf_pat_[s.len_pat_] = s.parent_byte_;
      ++s.len_pat_;
      break;
    case cas::Dimension::Value:
      buf_val_[s.len_val_] = s.parent_byte_;
      ++s.len_val_;
      break;
    case cas::Dimension::Leaf:
      assert(false);
      break;
    }
  }
  size_t node_pat_len = s.node_->separator_pos_;
  size_t node_val_len = s.node_->prefix_.size() - s.node_->separator_pos_;
  std::memcpy(&buf_pat_[s.len_pat_], &s.node_->prefix_[0],
      node_pat_len);
  std::memcpy(&buf_val_[s.len_val_], &s.node_->prefix_[s.node_->separator_pos_],
      node_val_len);
  s.len_pat_ += node_pat_len;
  s.len_val_ += node_val_len;
}


template<class VType>
cas::PathMatcher::PrefixMatch
cas::Query<VType>::MatchPathPrefix(State& s) {
  return pm_.MatchPathIncremental(buf_pat_, key_,
      s.len_pat_, s.pm_state_);
}


template<class VType>
cas::PathMatcher::PrefixMatch
cas::Query<VType>::MatchValuePrefix(State& s) {
  // match as much as possible of key_.low_
  while (s.vl_pos_ < key_.low_.size() &&
         s.vl_pos_ < s.len_val_ &&
         buf_val_[s.vl_pos_] == key_.low_[s.vl_pos_]) {
    ++s.vl_pos_;
  }
  // match as much as possible of key_.high_
  while (s.vh_pos_ < key_.high_.size() &&
         s.vh_pos_ < s.len_val_ &&
         buf_val_[s.vh_pos_] == key_.high_[s.vh_pos_]) {
    ++s.vh_pos_;
  }

  if (s.vl_pos_ < key_.low_.size() && s.vl_pos_ < s.len_val_ &&
      buf_val_[s.vl_pos_] < key_.low_[s.vl_pos_]) {
    // buf_val_ < key_.low_
    return PathMatcher::PrefixMatch::MISMATCH;
  }

  if (s.vh_pos_ < key_.high_.size() && s.vh_pos_ < s.len_val_ &&
      buf_val_[s.vh_pos_] > key_.high_[s.vh_pos_]) {
    // buf_val_ > key_.high_
    return PathMatcher::PrefixMatch::MISMATCH;
  }

  return IsCompleteValue(s) ? PathMatcher::PrefixMatch::MATCH
                            : PathMatcher::PrefixMatch::INCOMPLETE;
}


template<>
bool cas::Query<cas::vint32_t>::IsCompleteValue(State& s) {
  return s.len_val_ == sizeof(cas::vint32_t);
}
template<>
bool cas::Query<cas::vint64_t>::IsCompleteValue(State& s) {
  return s.len_val_ == sizeof(cas::vint64_t);
}
template<>
bool cas::Query<cas::vstring_t>::IsCompleteValue(State& s) {
  if (s.len_val_ <= 1) {
    // the null byte alone is no complete value
    return false;
  }
  return buf_val_[s.len_val_ - 1] == '\0';
}


template<class VType>
void cas::Query<VType>::Descend(State& s) {
  switch (s.node_->dimension_) {
  case cas::Dimension::Path:
    DescendPathNode(s);
    break;
  case cas::Dimension::Value:
    DescendValueNode(s);
    break;
  case cas::Dimension::Leaf:
    assert(false);
    break;
  }
}


template<class VType>
void cas::Query<VType>::DescendPathNode(State& s) {
  if (s.pm_state_.desc_qpos_ != -1 ||
      key_.path_[s.pm_state_.qpos_] == static_cast<uint8_t>(cas::PathMask::Descendant) ||
      key_.path_[s.pm_state_.qpos_] == static_cast<uint8_t>(cas::PathMask::Wildcard)) {
    // descend all children of s.node_
    s.node_->ForEachChild([&](uint8_t byte, cas::Node& child) -> bool {
      stack_.push_back({
        .node_             = &child,
        .parent_dimension_ = s.node_->dimension_,
        .parent_byte_      = byte,
        .len_pat_          = s.len_pat_,
        .len_val_          = s.len_val_,
        .pm_state_         = s.pm_state_,
        .vl_pos_           = s.vl_pos_,
        .vh_pos_           = s.vh_pos_,
      });
      return true;
    });
  } else {
    // we are looking for exactly one child
    uint8_t byte = key_.path_[s.pm_state_.qpos_];
    cas::Node* child = s.node_->LocateChild(byte);
    if (child != nullptr) {
      stack_.push_back({
        .node_             = child,
        .parent_dimension_ = s.node_->dimension_,
        .parent_byte_      = byte,
        .len_pat_          = s.len_pat_,
        .len_val_          = s.len_val_,
        .pm_state_         = s.pm_state_,
        .vl_pos_           = s.vl_pos_,
        .vh_pos_           = s.vh_pos_,
      });
    }
  }
}


template<class VType>
void cas::Query<VType>::DescendValueNode(State& s) {
  uint8_t low  = (s.vl_pos_ == s.len_val_) ? key_.low_[s.vl_pos_]  : 0x00;
  uint8_t high = (s.vh_pos_ == s.len_val_) ? key_.high_[s.vh_pos_] : 0xFF;
  s.node_->ForEachChild(low, high, [&](uint8_t byte, cas::Node& child) -> bool {
    stack_.push_back({
      .node_             = &child,
      .parent_dimension_ = s.node_->dimension_,
      .parent_byte_      = byte,
      .len_pat_          = s.len_pat_,
      .len_val_          = s.len_val_,
      .pm_state_         = s.pm_state_,
      .vl_pos_           = s.vl_pos_,
      .vh_pos_           = s.vh_pos_,
    });
    return true;
  });
}


template<class VType>
void cas::Query<VType>::EmitMatch(State& s) {
  assert(s.node_->IsLeaf());
  auto* leaf = static_cast<cas::Node0*>(s.node_);
  for (cas::ref_t& ref : leaf->refs_) {
    ++stats_.nr_matches_;
    emitter_(buf_pat_, buf_val_, ref);
  }
}


template<class VType>
void cas::Query<VType>::State::Dump() {
  std::cout << "node: " << node_ << std::endl;
  switch (parent_dimension_) {
  case cas::Dimension::Path:
    std::cout << "parent_dimension_: Path" << std::endl;
    break;
  case cas::Dimension::Value:
    std::cout << "parent_dimension_: Value" << std::endl;
    break;
  case cas::Dimension::Leaf:
    assert(false);
    break;
  }
  printf("parent_byte_: 0x%02X\n", static_cast<unsigned char>(parent_byte_)); // NOLINT
  std::cout << "len_val_: " << len_val_ << std::endl;
  std::cout << "len_pat_: " << len_pat_ << std::endl;
  pm_state_.Dump();
  std::cout << "vl_pos_: " << vl_pos_ << std::endl;
  std::cout << "vh_pos_: " << vh_pos_ << std::endl;
}


template<class VType>
void cas::Query<VType>::DumpState(State& s) {
  std::cout << "buf_pat_: ";
  cas::Utils::DumpChars(buf_pat_, s.len_pat_);
  std::cout << std::endl;
  std::cout << "buf_pat_: ";
  cas::Utils::DumpHexValues(buf_pat_, s.len_pat_);
  std::cout << std::endl;
  std::cout << "buf_val_: ";
  cas::Utils::DumpHexValues(buf_val_, s.len_val_);
  std::cout << std::endl;
  std::cout << "Sate:" << std::endl;
  s.Dump();
  std::cout << std::endl;
}


// explicit instantiations to separate header from implementation
template class cas::Query<cas::vint32_t>;
template class cas::Query<cas::vint64_t>;
template class cas::Query<cas::vstring_t>;
