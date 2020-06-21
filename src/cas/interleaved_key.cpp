#include "cas/interleaved_key.hpp"
#include <iostream>
#include <cassert>


void cas::InterleavedKey::Dump() const {
  std::cout << "ref_:   " << ref_ << std::endl;
  std::cout << "bytes_: ";
  for (const InterleavedByte& byte : bytes_) {
    char type;
    switch (byte.dimension_) {
      case cas::Dimension::Path:  type = 'P'; break;
      case cas::Dimension::Value: type = 'V'; break;
      default: assert(false);
    }
    printf("0x%02X%c ", (unsigned char) byte.byte_, type);
  }
  std::cout << std::endl;
}
