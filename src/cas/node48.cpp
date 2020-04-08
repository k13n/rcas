#include "cas/node48.hpp"
#include "cas/node256.hpp"
#include <cassert>
#include <iostream>


cas::Node48::Node48(cas::Dimension dimension)
    : cas::Node(dimension) {
  memset(indexes_, kEmptyIndex, 256*sizeof(uint8_t));
  memset(children_, 0, 48*sizeof(uintptr_t));
}


void cas::Node48::Put(uint8_t key_byte, Node* child) {
  if (nr_children_ >= 48) {
    // TODO
    exit(-1);
  }
  int pos = 0;
  while (pos < 48 && children_[pos] != nullptr) {
    ++pos;
  }
  indexes_[key_byte] = pos;
  children_[pos] = child;
  ++nr_children_;
}


cas::Node* cas::Node48::LocateChild(uint8_t key_byte) {
  uint8_t index = indexes_[key_byte];
  if (index == cas::kEmptyIndex) {
    return nullptr;
  } else {
    return children_[index];
  }
}


cas::Node* cas::Node48::Grow() {
  cas::Node256* node256 = new cas::Node256(dimension_);
  node256->nr_children_ = 48;
  node256->separator_pos_ = separator_pos_;
  node256->prefix_ = std::move(prefix_);
  for (int i = 0; i < 256; ++i) {
    if (indexes_[i] != cas::kEmptyIndex) {
      node256->children_[i] = children_[indexes_[i]];
    }
  }
  return node256;
}


bool cas::Node48::IsFull() {
  return nr_children_ >= 48;
}


void cas::Node48::ReplaceBytePointer(uint8_t key_byte, cas::Node* child) {
  uint8_t pos = indexes_[key_byte];
  if (pos == cas::kEmptyIndex) {
    assert(false);
  }
  children_[pos] = child;
}


void cas::Node48::ForEachChild(uint8_t low, uint8_t high,
                                 cas::ChildIt callback) {
  for (int i = high; i >= low; --i) {
    if (indexes_[i] != cas::kEmptyIndex) {
      callback(static_cast<uint8_t>(i), *children_[indexes_[i]]);
    }
  }
}


size_t cas::Node48::SizeBytes() {
  return sizeof(cas::Node48) + (prefix_.capacity() * sizeof(uint8_t));
}


void cas::Node48::Dump() {
  std::cout << "type: Node48" << std::endl;
  cas::Node::Dump();
  std::cout << "keys_: ";
  DumpIndexes();
  std::cout << std::endl;
  std::cout << "children_: ";
  DumpAddresses(children_, 48);
  std::cout << std::endl;
  std::cout << std::endl;
}


void cas::Node48::DumpIndexes() {
  for (int i = 0; i < 256; ++i) {
    if (indexes_[i] != cas::kEmptyIndex) {
      printf("0x%02X(%d)", i, indexes_[i]);
      if (i < 256-1) {
        std::cout << " ";
      }
    }
  }
}


int cas::Node48::NodeWidth() {
  return 48;
}
