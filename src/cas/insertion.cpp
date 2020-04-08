#include "cas/insertion.hpp"
#include "cas/node.hpp"
#include "cas/node0.hpp"
#include "cas/node4.hpp"

#include <cassert>
#include <iostream>


cas::Insertion::Insertion(cas::Node** root, cas::BinaryKey& bkey,
                          cas::Dimension root_dimension) :
    root_(root),
    bkey_(bkey),
    root_dimension_(root_dimension),
    loc_(*root, bkey)
  {}


bool cas::Insertion::Execute() {
  if (*root_ == nullptr) {
    *root_ = new cas::Node0(bkey_, 0, 0);
    return true;
  }

  loc_.Locate();

  if (loc_.matcher_.KeyFullyMatched()) {
    return AddRefToExistingLeaf();
  } else if (loc_.matcher_.HasMismatch()) {
    cas::Node* new_intermediate = SplitNode();
    if (loc_.parent_ == nullptr) {
      *root_ = new_intermediate;
    } else {
      loc_.parent_->ReplaceBytePointer(loc_.parent_byte_, new_intermediate);
    }
    return true;
  } else if (!loc_.node_->IsLeaf()) {
    // an inner node must be extended with a new child
    AddNewLeaf();
    return true;
  } else {
    // bkey_ is a prefix of an existing key
    return false;
  }
}


cas::Node* cas::Insertion::SplitNode() {
  if (loc_.matcher_.value_mismatch_ && loc_.matcher_.path_mismatch_) {
    if (loc_.parent_ == nullptr) {
      // loc_.node_ is the root node
      switch (loc_.node_->dimension_) {
      case cas::Dimension::Path:
        return SplitByValue();
      case cas::Dimension::Value:
        return SplitByPath();
      case cas::Dimension::Leaf:
        return (root_dimension_ == cas::Dimension::Path)
               ? SplitByPath() : SplitByValue();
      default:
        assert(false);
        return nullptr;
      }
    } else {
      switch (loc_.parent_->dimension_) {
      case cas::Dimension::Path:
        return SplitByValue();
      case cas::Dimension::Value:
        return SplitByPath();
      default:
        assert(false);
        return nullptr;
      }
    }
  } else if (loc_.matcher_.path_mismatch_) {
    return SplitByPath();
  } else { // loc_.matcher_.value_mismatch_
    return SplitByValue();
  }
}


cas::Node* cas::Insertion::SplitByPath() {
  std::vector<uint8_t> prefix = loc_.node_->prefix_;
  loc_.node_->prefix_.clear();

  // creaating the new sibling, setting its prefix
  cas::Node* new_sibling = new cas::Node0(
      bkey_,
      loc_.matcher_.bkey_p_pos_+1,
      loc_.matcher_.bkey_v_pos_);

  // setting the prefix of new_parent
  cas::Node* new_parent = new cas::Node4(cas::Dimension::Path);
  std::copy(
      prefix.begin(),
      prefix.begin() + loc_.matcher_.node_p_pos_,
      std::back_inserter(new_parent->prefix_));
  new_parent->separator_pos_ = new_parent->prefix_.size();
  std::copy(
      prefix.begin() + loc_.node_->separator_pos_,
      prefix.begin() + loc_.matcher_.node_v_pos_,
      std::back_inserter(new_parent->prefix_));

  // setting the prefix of node_
  std::copy(
      prefix.begin() + loc_.matcher_.node_p_pos_ + 1,
      prefix.begin() + loc_.node_->separator_pos_,
      std::back_inserter(loc_.node_->prefix_));
  loc_.node_->separator_pos_ = loc_.node_->prefix_.size();
  std::copy(
      prefix.begin() + loc_.matcher_.node_v_pos_,
      prefix.end(),
      std::back_inserter(loc_.node_->prefix_));

  // find the discriminator bytes that distinguish node_ and new_sibling
  uint8_t discriminator_sibling = bkey_.path_[loc_.matcher_.bkey_p_pos_];
  uint8_t discriminator_node    = prefix[loc_.matcher_.node_p_pos_];

  // link node_ and new_sibling as children of new_parent
  new_parent->Put(discriminator_node, loc_.node_);
  new_parent->Put(discriminator_sibling, new_sibling);

  return new_parent;
}


cas::Node* cas::Insertion::SplitByValue() {
  std::vector<uint8_t> prefix = loc_.node_->prefix_;
  loc_.node_->prefix_.clear();

  // creaating the new sibling, setting its prefix
  cas::Node* new_sibling = new cas::Node0(
      bkey_,
      loc_.matcher_.bkey_p_pos_,
      loc_.matcher_.bkey_v_pos_+1);

  // setting the prefix of new_parent
  cas::Node* new_parent = new cas::Node4(cas::Dimension::Value);
  std::copy(
      prefix.begin(),
      prefix.begin() + loc_.matcher_.node_p_pos_,
      std::back_inserter(new_parent->prefix_));
  new_parent->separator_pos_ = new_parent->prefix_.size();
  std::copy(
      prefix.begin() + loc_.node_->separator_pos_,
      prefix.begin() + loc_.matcher_.node_v_pos_,
      std::back_inserter(new_parent->prefix_));

  // setting the prefix of node_
  std::copy(
      prefix.begin() + loc_.matcher_.node_p_pos_,
      prefix.begin() + loc_.node_->separator_pos_,
      std::back_inserter(loc_.node_->prefix_));
  loc_.node_->separator_pos_ = loc_.node_->prefix_.size();
  std::copy(
      prefix.begin() + loc_.matcher_.node_v_pos_ + 1,
      prefix.end(),
      std::back_inserter(loc_.node_->prefix_));

  // find the discriminator bytes that distinguish node_ and new_sibling
  uint8_t discriminator_sibling = bkey_.value_[loc_.matcher_.bkey_v_pos_];
  uint8_t discriminator_node    = prefix[loc_.matcher_.node_v_pos_];

  // link node_ and new_sibling as children of new_parent
  new_parent->Put(discriminator_node, loc_.node_);
  new_parent->Put(discriminator_sibling, new_sibling);

  return new_parent;
}


void cas::Insertion::AddNewLeaf() {
  cas::Node* new_leaf = nullptr;
  uint8_t discriminator_byte = 0x00;

  switch (loc_.node_->Type()) {
  case cas::Dimension::Path:
    discriminator_byte = bkey_.path_[loc_.matcher_.bkey_p_pos_];
    new_leaf = new cas::Node0(bkey_,
        loc_.matcher_.bkey_p_pos_+1, loc_.matcher_.bkey_v_pos_);
    break;
  case cas::Dimension::Value:
    discriminator_byte = bkey_.value_[loc_.matcher_.bkey_v_pos_];
    new_leaf = new cas::Node0(bkey_,
        loc_.matcher_.bkey_p_pos_, loc_.matcher_.bkey_v_pos_+1);
    break;
  case cas::Dimension::Leaf:
    assert(false);
    break;
  }

  if (loc_.node_->IsFull()) {
    cas::Node* new_node = loc_.node_->Grow();
    new_node->Put(discriminator_byte, new_leaf);
    if (loc_.parent_ == nullptr) {
      *root_ = new_node;
    } else {
      loc_.parent_->ReplaceBytePointer(loc_.parent_byte_, new_node);
    }
    delete loc_.node_;
  } else {
    loc_.node_->Put(discriminator_byte, new_leaf);
  }
}


bool cas::Insertion::AddRefToExistingLeaf() {
  if (!loc_.node_->IsLeaf()) {
    return false;
  }
  cas::Node0* leaf = static_cast<cas::Node0*>(loc_.node_);
  // TODO: what if ref_ appears multiple times?
  leaf->refs_.push_back(bkey_.ref_);
  return true;
}
