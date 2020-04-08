#ifndef CAS_PREFIX_MATCHER_H_
#define CAS_PREFIX_MATCHER_H_

#include "cas/binary_key.hpp"
#include "cas/node.hpp"


namespace cas {

class PrefixMatcher {
public:
  Node* node_;
  BinaryKey* bkey_;
  size_t bkey_p_pos_ = 0;
  size_t bkey_v_pos_ = 0;
  size_t node_p_pos_ = 0;
  size_t node_v_pos_ = 0;
  bool path_mismatch_  = false;
  bool value_mismatch_ = false;

  void Match(Node* node,
      BinaryKey* bkey,
      size_t bkey_p_pos,
      size_t bkey_v_pos);

  bool HasMismatch();

  bool PrefixFullyMatched();

  bool KeyFullyMatched();

  void Dump();

private:
  void MatchValue();

  void MatchPath();
};


} // namespace cas

#endif // CAS_PREFIX_MATCHER_H_

