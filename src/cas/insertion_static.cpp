#include "cas/insertion_static.hpp"
#include "cas/node.hpp"
#include "cas/node0.hpp"
#include "cas/node4.hpp"
#include <iostream>
#include <cassert>


cas::InsertionStatic::InsertionStatic(cas::Node** root,
                                      cas::InterleavedKey& ikey) :
    root_(root),
    ikey_(ikey) {
}


bool cas::InsertionStatic::Execute() {
  if (*root_ == nullptr) {
    *root_ = new cas::Node0(ikey_, 0);
    return true;
  }

  Descend();
  if (KeyFullyMatched()) {
    return AddRefToExistingLeaf();
  } else if (NodePrefixMismatch()) {
    cas::Node* new_intermediate = SplitNode();
    if (parent_ == nullptr) {
      *root_ = new_intermediate;
    } else {
      parent_->ReplaceBytePointer(parent_byte_, new_intermediate);
    }
    return true;
  } else if (!node_->IsLeaf()) {
    // an inner node must be extended with a new child
    AddNewLeaf();
    return true;
  } else {
    // ikey_ is a prefix of an existing key
    return false;
  }

  return true;
}


void cas::InsertionStatic::Descend() {
  node_ = *root_;
  parent_ = nullptr;
  parent_byte_ = 0x00;

  while (true) {
    MatchPrefix();
    if (NodePrefixMismatch() || KeyFullyMatched()) {
      return;
    }
    uint8_t next_byte = ikey_.bytes_[ikey_pos_].byte_;
    cas::Node* child = node_->LocateChild(next_byte);
    if (child == nullptr) {
      return;
    }
    parent_byte_ = next_byte;
    parent_ = node_;
    node_ = child;
    ++ikey_pos_;
  }
}


void cas::InsertionStatic::MatchPrefix() {
  p_pos_ = 0;
  v_pos_ = node_->separator_pos_;

  while (ikey_pos_ < ikey_.bytes_.size()) {
    InterleavedByte& ib = ikey_.bytes_[ikey_pos_];
    switch (ib.dimension_) {
    case cas::Dimension::Path:
      if (p_pos_ < node_->separator_pos_ && node_->prefix_[p_pos_] == ib.byte_) {
        ++p_pos_;
      } else {
        return;
      }
      break;
    case cas::Dimension::Value:
      if (v_pos_ < node_->prefix_.size() && node_->prefix_[v_pos_] == ib.byte_) {
        ++v_pos_;
      } else {
        return;
      }
      break;
    default:
      assert(false);
    }
    ++ikey_pos_;
  }
}


bool cas::InsertionStatic::AddRefToExistingLeaf() {
  if (!node_->IsLeaf()) {
    return false;
  }
  cas::Node0* leaf = static_cast<cas::Node0*>(node_);
  // TODO: what if ref_ appears multiple times?
  leaf->refs_.push_back(ikey_.ref_);
  return true;
}



cas::Node* cas::InsertionStatic::SplitNode() {
  switch (ikey_.bytes_[ikey_pos_].dimension_) {
    case cas::Dimension::Path:  return SplitNodeByPath();
    case cas::Dimension::Value: return SplitNodeByValue();
    default: assert(false);
  }
  return nullptr;
}


cas::Node* cas::InsertionStatic::SplitNodeByPath() {
  const auto prefix = node_->prefix_;
  node_->prefix_.clear();

  // creaating the new sibling, setting its prefix
  cas::Node* new_sibling = new cas::Node0(ikey_, ikey_pos_+1);

  // setting the prefix of new_parent
  cas::Node* new_parent = new cas::Node4(cas::Dimension::Path);
  std::copy(
      prefix.begin(),
      prefix.begin() + p_pos_,
      std::back_inserter(new_parent->prefix_));
  new_parent->separator_pos_ = new_parent->prefix_.size();
  std::copy(
      prefix.begin() + node_->separator_pos_,
      prefix.begin() + v_pos_,
      std::back_inserter(new_parent->prefix_));

  // setting the prefix of node_
  std::copy(
      prefix.begin() + p_pos_ + 1,
      prefix.begin() + node_->separator_pos_,
      std::back_inserter(node_->prefix_));
  node_->separator_pos_ = node_->prefix_.size();
  std::copy(
      prefix.begin() + v_pos_,
      prefix.end(),
      std::back_inserter(node_->prefix_));

  // find the discriminator bytes that distinguish node_ and new_sibling
  uint8_t discriminator_sibling = ikey_.bytes_[ikey_pos_].byte_;
  uint8_t discriminator_node    = prefix[p_pos_];

  // link node_ and new_sibling as children of new_parent
  new_parent->Put(discriminator_node, node_);
  new_parent->Put(discriminator_sibling, new_sibling);

  return new_parent;
}


cas::Node* cas::InsertionStatic::SplitNodeByValue() {
  const auto prefix = node_->prefix_;
  node_->prefix_.clear();

  // creaating the new sibling, setting its prefix
  cas::Node* new_sibling = new cas::Node0(ikey_, ikey_pos_+1);

  // setting the prefix of new_parent
  cas::Node* new_parent = new cas::Node4(cas::Dimension::Value);
  std::copy(
      prefix.begin(),
      prefix.begin() + p_pos_,
      std::back_inserter(new_parent->prefix_));
  new_parent->separator_pos_ = new_parent->prefix_.size();
  std::copy(
      prefix.begin() + node_->separator_pos_,
      prefix.begin() + v_pos_,
      std::back_inserter(new_parent->prefix_));

  // setting the prefix of node_
  std::copy(
      prefix.begin() + p_pos_,
      prefix.begin() + node_->separator_pos_,
      std::back_inserter(node_->prefix_));
  node_->separator_pos_ = node_->prefix_.size();
  std::copy(
      prefix.begin() + v_pos_ + 1,
      prefix.end(),
      std::back_inserter(node_->prefix_));

  // find the discriminator bytes that distinguish node_ and new_sibling
  uint8_t discriminator_sibling = ikey_.bytes_[ikey_pos_].byte_;
  uint8_t discriminator_node    = prefix[v_pos_];

  // link node_ and new_sibling as children of new_parent
  new_parent->Put(discriminator_node, node_);
  new_parent->Put(discriminator_sibling, new_sibling);

  return new_parent;
}


void cas::InsertionStatic::AddNewLeaf() {
  uint8_t discriminator_byte = ikey_.bytes_[ikey_pos_].byte_;
  cas::Node* new_leaf = new cas::Node0(ikey_, ikey_pos_+1);
  if (node_->IsFull()) {
    cas::Node* new_node = node_->Grow();
    new_node->Put(discriminator_byte, new_leaf);
    if (parent_ == nullptr) {
      *root_ = new_node;
    } else {
      parent_->ReplaceBytePointer(parent_byte_, new_node);
    }
    delete node_;
  } else {
    node_->Put(discriminator_byte, new_leaf);
  }
}


bool cas::InsertionStatic::KeyFullyMatched() {
  return node_->IsLeaf() &&
         p_pos_ == node_->separator_pos_ &&
         v_pos_ == node_->prefix_.size();
}


bool cas::InsertionStatic::NodePrefixMismatch() {
  return p_pos_ < node_->separator_pos_ ||
         v_pos_ < node_->prefix_.size();
}
