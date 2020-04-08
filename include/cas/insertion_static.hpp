#ifndef CAS_INSERTION_STATIC_H_
#define CAS_INSERTION_STATIC_H_

#include "cas/node.hpp"
#include "cas/interleaved_key.hpp"

namespace cas {


class InsertionStatic {
protected:
  cas::Node** root_;
  cas::InterleavedKey& ikey_;
  cas::Node* parent_ = nullptr;
  cas::Node* node_ = nullptr;
  uint8_t parent_byte_ = 0x00;
  size_t ikey_pos_ = 0;
  size_t p_pos_ = 0;
  size_t v_pos_ = 0;


public:
  InsertionStatic(cas::Node** root, cas::InterleavedKey& ikey);

  bool Execute();

private:
  void Descend();

  void MatchPrefix();

  bool AddRefToExistingLeaf();

  cas::Node* SplitNode();
  cas::Node* SplitNodeByPath();
  cas::Node* SplitNodeByValue();

  void AddNewLeaf();

  bool KeyFullyMatched();
  bool NodePrefixMismatch();
};


}; // namespace cas

#endif // CAS_INSERTION_STATIC_H_
