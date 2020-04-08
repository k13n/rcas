#ifndef CAS_LOCATOR_H_
#define CAS_LOCATOR_H_

#include "cas/binary_key.hpp"
#include "cas/prefix_matcher.hpp"

namespace cas {


class Locator {
public:
  Node* root_;
  BinaryKey& bkey_;
  Node* parent_ = nullptr;
  uint8_t parent_byte_ = 0x00;
  Node* node_ = nullptr;
  PrefixMatcher matcher_;


  Locator(Node* root, BinaryKey& bkey);

  void Locate();

  void Dump();
};


}; // namespace cas

#endif // CAS_LOCATOR_H_
