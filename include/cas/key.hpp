#ifndef CAS_KEY_H_
#define CAS_KEY_H_

#include "cas/types.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>


namespace cas {

template<class VType>
struct Key {
  std::string path_;
  VType value_;
  ref_t ref_;

  Key() = default;
  Key(std::string path, VType value, ref_t ref);

  void Dump() const;
};


template<class VType>
bool operator==(const Key<VType>& lhs, const Key<VType>& rhs) {
  return lhs.value_ == rhs.value_ &&
    lhs.path_ == rhs.path_ &&
    lhs.ref_ == rhs.ref_;
}


} // namespace cas

#endif // CAS_KEY_H_
