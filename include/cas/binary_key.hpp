#ifndef CAS_BINARY_KEY_H_
#define CAS_BINARY_KEY_H_

#include "cas/types.hpp"
#include "cas/dimension.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>


namespace cas {

struct BinaryKey {
  std::vector<uint8_t> path_;
  std::vector<uint8_t> value_;
  ref_t ref_ = 0;

  const std::vector<uint8_t>& Get(cas::Dimension dimension);

  void Dump() const;
};

} // namespace cas

#endif // CAS_BINARY_KEY_H_
