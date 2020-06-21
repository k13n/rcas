#ifndef CAS_SEARCH_KEY_H_
#define CAS_SEARCH_KEY_H_

#include "cas/key.hpp"
#include <vector>
#include <string>

namespace cas {

const std::string kAxisChild      = "axis:*";
const std::string kAxisDescendant = "axis:**";

const uint8_t kByteChild            = 0xFD;
const uint8_t kByteDescendantOrSelf = 0xFE;

template<class VType>
struct SearchKey {
  VType low_;
  VType high_;
  path_t path_;

  void Dump();

  void DumpConcise();
};


/* these are our pre-defined types */
using BoolSearchKey   = SearchKey<bool>;
using IntSearchKey    = SearchKey<int64_t>;
using DoubleSearchKey = SearchKey<double>;
using StringSearchKey = SearchKey<std::string>;


enum class ByteType {
  kTypeLabel,
  kTypePathSeperator,
  kTypeWildcard,
  kTypeDescendant,
};


struct BinaryQP {
  std::vector<uint8_t>  bytes_;
  std::vector<ByteType> types_;

  size_t Size() const;

  void Dump() const;
};


struct BinarySK {
  std::vector<uint8_t> low_;
  std::vector<uint8_t> high_;
  BinaryQP path_;

  void Dump();
};


} // namespace cas

#endif // CAS_SEARCH_KEY_H_
