#include "cas/node.hpp"
#include "cas/utils.hpp"
#include "cas/key_encoding.hpp"
#include "cas/node0.hpp"

#include <iostream>
#include <cctype>
#include <cassert>


cas::Node::Node(cas::Dimension dimension) : dimension_(dimension) {
}


cas::Dimension cas::Node::Type() const {
  return dimension_;
}


bool cas::Node::IsPathNode() const {
  return dimension_ == cas::Dimension::Path;
}


bool cas::Node::IsValueNode() const {
  return dimension_ == cas::Dimension::Value;
}


void cas::Node::ForEachChild(uint8_t low, const cas::ChildIt& callback) {
  return ForEachChild(low, 0xFF, callback);
}


void cas::Node::ForEachChild(const cas::ChildIt& callback) {
  return ForEachChild(0x00, 0xFF, callback);
}


size_t cas::Node::PathPrefixSize() const {
  return separator_pos_;
}


size_t cas::Node::ValuePrefixSize() const {
  return prefix_.size() - separator_pos_;
}


void cas::Node::CollectStats(cas::IndexStats& stats, size_t depth) {
  stats.size_bytes_ += SizeBytes();
  ++stats.nr_nodes_;
  if (IsPathNode()) {
    ++stats.nr_path_nodes_;
  }
  if (IsValueNode()) {
    ++stats.nr_value_nodes_;
  }
  if (depth > stats.max_depth_) {
    stats.max_depth_ = depth;
  }
  switch (NodeWidth()) {
  case 0:
    ++stats.nr_node0_;
    break;
  case 4:
    if (IsPathNode()) {
      ++stats.nr_p_node4_;
    } else {
      assert(IsValueNode()); // NOLINT
      ++stats.nr_v_node4_;
    }
    break;
  case 16:
    if (IsPathNode()) {
      ++stats.nr_p_node16_;
    } else {
      assert(IsValueNode()); // NOLINT
      ++stats.nr_v_node16_;
    }
    break;
  case 48:
    if (IsPathNode()) {
      ++stats.nr_p_node48_;
    } else {
      assert(IsValueNode()); // NOLINT
      ++stats.nr_v_node48_;
    }
    break;
  case 256:
    if (IsPathNode()) {
      ++stats.nr_p_node256_;
    } else {
      assert(IsValueNode()); // NOLINT
      ++stats.nr_v_node256_;
    }
    break;
  default:
    assert(false); // NOLINT
  }

  auto it = stats.depth_histo_.find(depth);
  if (it == stats.depth_histo_.end()) {
    stats.depth_histo_[depth] = 0;
  }
  ++stats.depth_histo_[depth];

  ForEachChild([&](uint8_t /* byte */, cas::Node& child) -> bool {
    if (IsPathNode() && child.IsPathNode()) {
      ++stats.pp_steps;
    } else if (IsPathNode() && child.IsValueNode()) {
      ++stats.pv_steps;
    } else if (IsValueNode() && child.IsPathNode()) {
      ++stats.vp_steps;
    } else if (IsValueNode() && child.IsValueNode()) {
      ++stats.vv_steps;
    }
    child.CollectStats(stats, depth + 1);
    return true;
  });
}


void cas::Node::Dump() {
  std::cout << "dimension_: ";
  switch (dimension_) {
  case cas::Dimension::Value:
    std::cout << "Value";
    break;
  case cas::Dimension::Path:
    std::cout << "Path";
    break;
  case cas::Dimension::Leaf:
    std::cout << "Leaf";
    break;
  }
  std::cout << std::endl;
  std::cout << "address: " << this << std::endl;
  std::cout << "nr_children_: " << static_cast<int>(nr_children_) << std::endl;
  std::cout << "prefix_length_: " << prefix_.size() << std::endl;
  std::cout << "separator_pos_: " << separator_pos_ << std::endl;
  std::cout << "prefix_: ";
  cas::Utils::DumpHexValues(prefix_);
  std::cout << std::endl;
}


void cas::Node::DumpRecursive() {
  Dump();
  ForEachChild([](uint8_t /* byte */, cas::Node& child) -> bool {
    child.DumpRecursive();
    return true;
  });
}


void cas::Node::DumpConcise(uint8_t edge_label, int indent) {
  std::string istring = std::string(2*indent, ' ');
  std::cout << istring;
  switch (dimension_) {
  case cas::Dimension::Path:
    std::cout << "[P] ";
    break;
  case cas::Dimension::Value:
    std::cout << "[V] ";
    break;
  case cas::Dimension::Leaf:
    std::cout << "[L] ";
    break;
  }
  if (std::isprint(edge_label) != 0) {
    printf("0x%02X(%1c): ", static_cast<unsigned char>(edge_label), edge_label); // NOLINT
  } else {
    printf("0x%02X():  ", static_cast<unsigned char>(edge_label)); // NOLINT
  }
  std::cout << "path[";
  /* cas::Utils::DumpChars(prefix_, separator_pos_); */
  cas::Utils::DumpHexValues(prefix_, separator_pos_);
  std::cout << "] ";
  std::cout << "value[";
  cas::Utils::DumpHexValues(prefix_, separator_pos_, prefix_.size());
  std::cout << "] ";
  // print capacity of prefixes
  std::cout << "cap(" << prefix_.capacity();
  if (IsLeaf()) {
    auto* self = dynamic_cast<cas::Node0*>(this);
    std::cout << ";" << self->refs_.size() << "," << self->refs_.capacity() << "*" << sizeof(cas::ref_t);
  }
  std::cout << ")";
  std::cout << std::endl;
  // recursively dump children (in ascending order)
  std::vector<std::pair<uint8_t, cas::Node*>> children;
  children.reserve(nr_children_);
  ForEachChild([&](uint8_t elabel, cas::Node& child) -> bool {
    children.emplace_back(std::make_pair(elabel, &child));
    return true;
  });
  for (auto i = children.rbegin(); i != children.rend(); ++i) {
    auto  elabel = std::get<0>(*i);
    auto* child  = std::get<1>(*i);
    child->DumpConcise(elabel, indent+1);
  }
}


void cas::Node::DumpBuffer(uint8_t *buffer, size_t length) {
  for (size_t i = 0; i < length; ++i) {
    printf("0x%02X", static_cast<unsigned char>(buffer[i])); // NOLINT
    if (i < length-1) {
      printf(" "); // NOLINT
    }
  }
}


void cas::Node::DumpAddresses(cas::Node **buffer, size_t length) {
  for (size_t i = 0; i < length; ++i) {
    printf("%p", buffer[i]); // NOLINT
    if (i < length-1) {
      printf(" "); // NOLINT
    }
  }
}
