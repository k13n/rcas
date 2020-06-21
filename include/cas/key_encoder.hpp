#ifndef CAS_KEY_ENCODER_H_
#define CAS_KEY_ENCODER_H_

#include "cas/key.hpp"
#include "cas/search_key.hpp"
#include "cas/binary_key.hpp"
#include "cas/surrogate.hpp"
#include <cstring>
#include <optional>

namespace cas {

template<class VType>
class KeyEncoder {
public:
  BinaryKey Encode(const Key<VType>& key);

  BinaryKey Encode(const Key<VType>& key, Surrogate& surrogate);

  BinarySK Encode(SearchKey<VType>& key);

  BinarySK Encode(SearchKey<VType>& key, Surrogate& surrogate);

private:
  void ReserveSpace(const Key<VType>& key, BinaryKey& bkey);

  void EncodePath(const Key<VType>& key, BinaryKey& bkey);

  void EncodePath(const Key<VType>& key, BinaryKey& bkey,
      Surrogate& surrogate);

  void EncodeValue(const Key<VType>& key, BinaryKey& bkey);

  void EncodeValue(const VType& value, std::vector<uint8_t>& buffer);

  size_t ValueSize(const VType& value);

  void EncodeQueryPath(SearchKey<VType>& key, std::vector<uint8_t>& buffer);

  void EncodeQueryPath(SearchKey<VType>& key, BinarySK& skey,
      Surrogate& surrogate);

  static inline void MemCpyToBuffer(std::vector<uint8_t>& buffer, int& offset,
      const void* value, std::size_t size) {
    std::memcpy(&buffer[offset], value, size);
    offset += size;
  }
};

} // namespace cas

#endif // CAS_KEY_ENCODER_H_
