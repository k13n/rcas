#ifndef CAS_NODE48_H_
#define CAS_NODE48_H_

#include "cas/node.hpp"


namespace cas {

const int kEmptyIndex = 0xFF;

class Node48 : public Node {
public:
  uint8_t indexes_[256];
  Node* children_[48];

  Node48(Dimension dimension);

  bool IsFull() override;

  void Put(uint8_t key_byte, Node* child) override;

  Node* LocateChild(uint8_t key_byte) override;

  Node* Grow() override;

  void ReplaceBytePointer(uint8_t key_byte, Node* child) override;

  void ForEachChild(uint8_t low, uint8_t high, ChildIt callback) override;

  size_t SizeBytes() override;

  void Dump() override;

  int NodeWidth() override;

private:
  void DumpIndexes();
};

} // namespace cas

#endif  // CAS_NODE48_H_
