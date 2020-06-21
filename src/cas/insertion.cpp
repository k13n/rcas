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
    cas::Node* new_intermediate = Restructure();
    /* cas::Node* new_intermediate = SplitNode(); */
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


cas::Node* cas::Insertion::Restructure() {
  cas::Dimension d = DetermineDimension();
  int offset_p = (d == cas::Dimension::Path)  ? 1 : 0;
  int offset_v = (d == cas::Dimension::Value) ? 1 : 0;

  std::vector<uint8_t> prefix = loc_.node_->prefix_;
  loc_.node_->prefix_.clear();

  // creating the new parent, setting its prefix
  cas::Node* new_parent = new cas::Node4(d);
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
      prefix.begin() + loc_.matcher_.node_p_pos_ + offset_p,
      prefix.begin() + loc_.node_->separator_pos_,
      std::back_inserter(loc_.node_->prefix_));
  loc_.node_->separator_pos_ = loc_.node_->prefix_.size();
  std::copy(
      prefix.begin() + loc_.matcher_.node_v_pos_ + offset_v,
      prefix.end(),
      std::back_inserter(loc_.node_->prefix_));

  // creaating the new sibling, setting its prefix
  cas::Node* new_sibling = new cas::Node0(bkey_,
      loc_.matcher_.bkey_p_pos_ + offset_p,
      loc_.matcher_.bkey_v_pos_ + offset_v);

  // find the values of the discriminative bytes that distinguish
  // node_ and new_sibling
  uint8_t dv_node, dv_sibling;
  if (d == cas::Dimension::Path) {
    dv_node = prefix[loc_.matcher_.node_p_pos_];
    dv_sibling = bkey_.path_[loc_.matcher_.bkey_p_pos_];
  } else {
    dv_node = prefix[loc_.matcher_.node_v_pos_];
    dv_sibling = bkey_.value_[loc_.matcher_.bkey_v_pos_];
  }

  // link node_ and new_sibling as children of new_parent
  new_parent->Put(dv_node, loc_.node_);
  new_parent->Put(dv_sibling, new_sibling);

  return new_parent;
}


cas::Dimension cas::Insertion::DetermineDimension() {
  if (loc_.matcher_.value_mismatch_ && loc_.matcher_.path_mismatch_) {
    if (loc_.parent_ == nullptr) {
      // loc_.node_ is the root node
      switch (loc_.node_->dimension_) {
      case cas::Dimension::Path:  return cas::Dimension::Value;
      case cas::Dimension::Value: return cas::Dimension::Path;
      case cas::Dimension::Leaf:
        return (root_dimension_ == cas::Dimension::Path)
               ? cas::Dimension::Path : cas::Dimension::Value;
      }
    } else {
      switch (loc_.parent_->dimension_) {
      case cas::Dimension::Path:  return cas::Dimension::Value;
      case cas::Dimension::Value: return cas::Dimension::Path;
      default:
        assert(false);
        throw std::runtime_error("Impossible");
      }
    }
  } else if (loc_.matcher_.path_mismatch_) {
    return cas::Dimension::Path;
  } else { // loc_.matcher_.value_mismatch_
    return cas::Dimension::Value;
  }
}


void cas::Insertion::AddNewLeaf() {
  cas::Node* new_leaf = nullptr;
  uint8_t discriminative_byte = 0x00;

  switch (loc_.node_->Type()) {
  case cas::Dimension::Path:
    discriminative_byte = bkey_.path_[loc_.matcher_.bkey_p_pos_];
    new_leaf = new cas::Node0(bkey_,
        loc_.matcher_.bkey_p_pos_+1, loc_.matcher_.bkey_v_pos_);
    break;
  case cas::Dimension::Value:
    discriminative_byte = bkey_.value_[loc_.matcher_.bkey_v_pos_];
    new_leaf = new cas::Node0(bkey_,
        loc_.matcher_.bkey_p_pos_, loc_.matcher_.bkey_v_pos_+1);
    break;
  case cas::Dimension::Leaf:
    assert(false);
    break;
  }

  if (loc_.node_->IsFull()) {
    cas::Node* new_node = loc_.node_->Grow();
    new_node->Put(discriminative_byte, new_leaf);
    if (loc_.parent_ == nullptr) {
      *root_ = new_node;
    } else {
      loc_.parent_->ReplaceBytePointer(loc_.parent_byte_, new_node);
    }
    delete loc_.node_;
  } else {
    loc_.node_->Put(discriminative_byte, new_leaf);
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
