#ifndef CAS_INTERLEAVED_KEY_H_
#define CAS_INTERLEAVED_KEY_H_

#include "cas/dimension.hpp"
#include "cas/types.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>


namespace cas {

struct InterleavedByte {
  uint8_t byte_;
  Dimension dimension_;
};


struct InterleavedKey {
  std::vector<InterleavedByte> bytes_;
  ref_t ref_;

  void Dump() const;
};


} // namespace cas

#endif // CAS_INTERLEAVED_KEY_H_
