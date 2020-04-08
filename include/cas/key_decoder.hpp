#ifndef CAS_KEY_DECODER_H_
#define CAS_KEY_DECODER_H_

#include "cas/key.hpp"
#include "cas/key_encoding.hpp"
#include "cas/surrogate.hpp"

#include <cstring>


namespace cas {

template<class VType>
class KeyDecoder {

public:
  KeyDecoder() {}

  Key<VType> Decode(
      const std::vector<uint8_t>& buffer_path,
      const std::vector<uint8_t>& buffer_value,
      ref_t ref);

  Key<VType> Decode(
      Surrogate& surrogate,
      const std::vector<uint8_t>& buffer_path,
      const std::vector<uint8_t>& buffer_value,
      ref_t ref);

private:
  void DecodeValue(Key<VType>& key, const std::vector<uint8_t>& buffer, int& offset);

  void DecodePath(Key<VType>& key, const std::vector<uint8_t>& buffer, int& offset);

  void DecodePath(Key<VType>& key, const std::vector<uint8_t>& buffer,
      Surrogate& surrogate);

  static inline void MemCpyFromBuffer(const std::vector<uint8_t>& buffer, int& offset,
      void* value, std::size_t size) {
    std::memcpy(value, &buffer[offset], size);
    offset += size;
  }
};

} // namespace cas

#endif // CAS_KEY_DECODER_H_
