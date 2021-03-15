#include "cas/node0.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>


cas::Node0::Node0() : cas::Node(cas::Dimension::Leaf) {}


cas::Node0::Node0(const cas::BinaryKey& bkey, size_t path_pos, size_t value_pos)
    : cas::Node(cas::Dimension::Leaf) {
  size_t size = (bkey.path_.size() - path_pos) +
                (bkey.value_.size() - value_pos);
  prefix_.reserve(size);
  // fill prefix_ with the remaining value bytes
  std::copy(bkey.path_.begin() + path_pos, bkey.path_.end(),
      std::back_inserter(prefix_));
  // fill prefix_ with the remaining path bytes
  separator_pos_ = prefix_.size();
  std::copy(bkey.value_.begin() + value_pos, bkey.value_.end(),
      std::back_inserter(prefix_));
  // add the ref
  refs_.push_back(bkey.ref_);
}


cas::Node0::Node0(const InterleavedKey& ikey, size_t pos)
    : cas::Node(cas::Dimension::Leaf) {
  assert(ikey.bytes_.size() >= pos); // NOLINT
  prefix_.reserve(ikey.bytes_.size() - pos);

  for (size_t i = pos; i < ikey.bytes_.size(); ++i) {
    if (ikey.bytes_[i].dimension_ == Dimension::Path) {
      prefix_.push_back(ikey.bytes_[i].byte_);
    }
  }
  separator_pos_ = prefix_.size();
  for (size_t i = pos; i < ikey.bytes_.size(); ++i) {
    if (ikey.bytes_[i].dimension_ == Dimension::Value) {
      prefix_.push_back(ikey.bytes_[i].byte_);
    }
  }

  refs_.push_back(ikey.ref_);
}


void cas::Node0::Put(uint8_t /*key_byte*/, cas::Node* /*child*/) {
  // TODO(@kevin)
  exit(-1);
}


cas::Node* cas::Node0::LocateChild(uint8_t /*key_byte*/) {
  return nullptr;
}


bool cas::Node0::ContainsRef(cas::ref_t ref) {
  return std::find(refs_.begin(), refs_.end(), ref) != refs_.end();
}


void cas::Node0::ReplaceBytePointer(uint8_t /*byte*/, cas::Node* /*child*/) {
  assert(false); // NOLINT
}


void cas::Node0::ForEachChild(uint8_t /*low*/, uint8_t /*high*/,
                              const cas::ChildIt& /*callback*/) {
  // has no children
}


size_t cas::Node0::SizeBytes() {
  return sizeof(cas::Node0) +
    (prefix_.capacity() * sizeof(uint8_t)) +
    (refs_.capacity() * sizeof(cas::ref_t));
}


void cas::Node0::Dump() {
  std::cout << "type: Node0" << std::endl;
  cas::Node::Dump();
  std::cout << "Refs: ";
  for (const auto& ref : refs_) {
    std::cout << cas::RefToString(ref) << " ";
  }
  std::cout << std::endl;
  std::cout << std::endl;
}


int cas::Node0::NodeWidth() {
  return 0;
}
