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

  bool IsFull();

  void Put(uint8_t key_byte, Node* child);

  Node* LocateChild(uint8_t key_byte);

  Node* Grow();

  void ReplaceBytePointer(uint8_t key_byte, Node* child);

  void ForEachChild(uint8_t low, uint8_t high, ChildIt callback);

  size_t SizeBytes();

  void Dump();

  int NodeWidth();

private:
  void DumpIndexes();
};

} // namespace cas

#endif  // CAS_NODE48_H_
