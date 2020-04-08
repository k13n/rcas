#include "cas/prefix_matcher.hpp"
#include <iostream>


bool cas::PrefixMatcher::HasMismatch() {
  return path_mismatch_ || value_mismatch_;
}


bool cas::PrefixMatcher::PrefixFullyMatched() {
  return node_p_pos_ >= node_->separator_pos_ &&
         node_v_pos_ >= node_->prefix_.size();
}


bool cas::PrefixMatcher::KeyFullyMatched() {
  return bkey_p_pos_ >= bkey_->path_.size() &&
         bkey_v_pos_ >= bkey_->value_.size();
}


void cas::PrefixMatcher::Match(cas::Node* node, cas::BinaryKey* bkey,
      size_t bkey_p_pos, size_t bkey_v_pos) {
  if (node == nullptr || bkey == nullptr) {
    return;
  }
  node_ = node;
  bkey_ = bkey;
  bkey_p_pos_ = bkey_p_pos;
  bkey_v_pos_ = bkey_v_pos;
  MatchPath();
  MatchValue();
}


void cas::PrefixMatcher::MatchPath() {
  node_p_pos_ = 0;
  while (node_p_pos_ < node_->separator_pos_
      && bkey_p_pos_ < bkey_->path_.size()
      && bkey_->path_[bkey_p_pos_] == node_->prefix_[node_p_pos_]) {
    ++node_p_pos_;
    ++bkey_p_pos_;
  }
  path_mismatch_ = (node_p_pos_ < node_->separator_pos_);
}


void cas::PrefixMatcher::MatchValue() {
  node_v_pos_ = node_->separator_pos_;
  while (node_v_pos_ < node_->prefix_.size()
      && bkey_v_pos_ < bkey_->value_.size()
      && bkey_->value_[bkey_v_pos_] == node_->prefix_[node_v_pos_]) {
    ++node_v_pos_;
    ++bkey_v_pos_;
  }
  value_mismatch_ = (node_v_pos_ < node_->prefix_.size());
}


void cas::PrefixMatcher::Dump() {
  std::cout << "bkey_p_pos_: " << bkey_p_pos_ << std::endl;
  std::cout << "bkey_v_pos_: " << bkey_v_pos_ << std::endl;
  std::cout << "node_p_pos_: " << node_p_pos_ << std::endl;
  std::cout << "node_v_pos_: " << node_v_pos_ << std::endl;
  std::cout << "path_mismatch_:  " << path_mismatch_ << std::endl;
  std::cout << "value_mismatch_: " << value_mismatch_ << std::endl;
}
