#ifndef CAS_INTERLEAVER_H_
#define CAS_INTERLEAVER_H_

#include "cas/binary_key.hpp"
#include "cas/interleaved_key.hpp"
#include "cas/index_type.hpp"


namespace cas {

class Interleaver {
public:
  static InterleavedKey Interleave(const BinaryKey& bkey,
      const IndexType& type);

  static InterleavedKey LabelWise(const BinaryKey& bkey);

  static InterleavedKey PathValue(const BinaryKey& bkey);

  static InterleavedKey ValuePath(const BinaryKey& bkey);

  static InterleavedKey ZOrder(const BinaryKey& bkey);

};


} // namespace cas

#endif // CAS_INTERLEAVER_H_
