#ifndef CAS_NODE4_H_
#define CAS_NODE4_H_

#include "cas/node.hpp"


namespace cas {

class Node4 : public Node {
public:
  uint8_t keys_[4];
  Node* children_[4];

  Node4(Dimension dimension);

  bool IsFull() const override;

  void Put(uint8_t key_byte, Node* child) override;

  Node* LocateChild(uint8_t key_byte) override;

  Node* Grow() override;

  void ReplaceBytePointer(uint8_t key_byte, Node* child) override;

  void ForEachChild(uint8_t low, uint8_t high, const ChildIt& callback) override;

  size_t SizeBytes() override;

  void Dump() override;

  int NodeWidth() override;
};

} // namespace cas

#endif  // CAS_NODE4_H_
