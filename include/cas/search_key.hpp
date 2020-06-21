#ifndef CAS_SEARCH_KEY_H_
#define CAS_SEARCH_KEY_H_

#include "cas/key.hpp"
#include "cas/key_encoding.hpp"
#include <string>
#include <vector>

namespace cas {


template<class VType>
struct SearchKey {
  std::string path_;
  VType low_;
  VType high_;

  void Dump() const;
  void DumpConcise() const;
};


struct BinarySK {
  std::vector<uint8_t>  path_;
  std::vector<PathMask> mask_;
  std::vector<uint8_t>  low_;
  std::vector<uint8_t>  high_;

  void Dump() const;
};


} // namespace cas

#endif // CAS_SEARCH_KEY_H_
