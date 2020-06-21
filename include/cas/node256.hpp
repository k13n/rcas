#ifndef CAS_NODE256_H_
#define CAS_NODE256_H_

#include "cas/node.hpp"


namespace cas {

class Node256 : public Node {
public:
  Node* children_[256];

  Node256(Dimension dimension);

  bool IsFull() override;

  void Put(uint8_t key_byte, Node* child) override;

  Node* LocateChild(uint8_t key_byte) override;

  Node* Grow() override;

  void ReplaceBytePointer(uint8_t key_byte, Node* child) override;

  void ForEachChild(uint8_t low, uint8_t high, ChildIt callback) override;

  size_t SizeBytes() override;

  void Dump() override;

  int NodeWidth() override;
};

} // namespace cas

#endif  // CAS_NODE256_H_
