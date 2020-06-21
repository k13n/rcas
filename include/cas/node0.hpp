#ifndef CAS_NODE0_H_
#define CAS_NODE0_H_

#include "cas/types.hpp"
#include "cas/binary_key.hpp"
#include "cas/interleaved_key.hpp"
#include "cas/node.hpp"

namespace cas {


class Node0 : public Node {
public:
  std::vector<ref_t> refs_;

  Node0();

  Node0(const BinaryKey& bkey, size_t path_pos, size_t value_pos);

  Node0(const InterleavedKey& ikey, size_t pos);

  inline bool IsLeaf() override {
    return true;
  };

  inline bool IsFull() const override {
    return true;
  }

  void Put(uint8_t key_byte, Node *child) override;

  Node* LocateChild(uint8_t key_byte) override;

  inline Node* Grow() override {
    return nullptr;
  }

  void ReplaceBytePointer(uint8_t key_byte, Node* child) override;

  void ForEachChild(uint8_t low, uint8_t high, const ChildIt& callback) override;

  size_t SizeBytes() override;

  void Dump() override;

  bool ContainsRef(ref_t ref);

  int NodeWidth() override;
};


} // namespace cas

#endif  // CAS_NODE0_H_
