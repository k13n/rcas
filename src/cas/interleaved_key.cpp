#include "cas/interleaved_key.hpp"
#include <iostream>
#include <cassert>


void cas::InterleavedKey::Dump() const {
  std::cout << "ref_:   " << cas::RefToString(ref_) << std::endl;
  std::cout << "bytes_: ";
  for (const InterleavedByte& byte : bytes_) {
    char type = 0;
    switch (byte.dimension_) {
      case cas::Dimension::Path:  type = 'P'; break;
      case cas::Dimension::Value: type = 'V'; break;
      default: assert(false); // NOLINT
    }
    printf("0x%02X%c ", static_cast<unsigned char>(byte.byte_), type); // NOLINT
  }
  std::cout << std::endl;
}
