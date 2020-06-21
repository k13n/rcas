#include "cas/key_decoder.hpp"

#include <string>
#include <vector>


namespace cas {


template<class VType>
cas::Key<VType> cas::KeyDecoder<VType>::Decode(
      const std::vector<uint8_t>& buffer_path,
      const std::vector<uint8_t>& buffer_value,
      ref_t ref) {
  cas::Key<VType> key;
  // decode path
  int offset = 0;
  DecodePath(key, buffer_path, offset);
  // decode value
  offset = 0;
  DecodeValue(key, buffer_value, offset);
  // set ref
  key.ref_ = ref;
  return key;
}


template<class VType>
cas::Key<VType> cas::KeyDecoder<VType>::Decode(
      cas::Surrogate& surrogate,
      const std::vector<uint8_t>& buffer_path,
      const std::vector<uint8_t>& buffer_value,
      ref_t ref) {
  cas::Key<VType> key;
  // decode path
  DecodePath(key, buffer_path, surrogate);
  // decode value
  int offset = 0;
  DecodeValue(key, buffer_value, offset);
  // set ref
  key.ref_ = ref;
  return key;
}


template<class VType>
void cas::KeyDecoder<VType>::DecodePath(
    cas::Key<VType>& key,
    const std::vector<uint8_t>& buffer,
    int& offset) {
  size_t size = 0;
  while (buffer[offset+size] != kNullByte) {
    ++size;
  }
  key.path_.reserve(size);
  for (size_t i = 0; i < size; ++i) {
    if (buffer[offset+i] == static_cast<uint8_t>(cas::PathMask::PathSeperator)) {
      key.path_.push_back('/');
    } else {
      key.path_.push_back(buffer[offset+i]);
    }
  }
}


template<class VType>
void cas::KeyDecoder<VType>::DecodePath(
    cas::Key<VType>& key,
    const std::vector<uint8_t>& buffer,
    Surrogate& surrogate) {
  std::vector<uint8_t> bytes(surrogate.bytes_per_label_, 0x00);
  size_t pos = 0;
  while (pos < surrogate.NrBytes()) {
    for (size_t i = 0; i < surrogate.bytes_per_label_; ++i, ++pos) {
      bytes[i] = buffer[pos];
    }
    std::string label = surrogate.MapLabelInv(bytes);
    if (!label.empty()) {
      key.path_ += ("/" + label);
    }
  }
}


template<>
void cas::KeyDecoder<cas::vint32_t>::DecodeValue(
    cas::Key<cas::vint32_t>& key,
    const std::vector<uint8_t>& buffer,
    int& offset) {
  MemCpyFromBuffer(buffer, offset, &key.value_, sizeof(key.value_));
  key.value_ = __builtin_bswap32(key.value_);
  key.value_ ^= cas::kMsbMask32;
}


template<>
void cas::KeyDecoder<cas::vint64_t>::DecodeValue(
    cas::Key<cas::vint64_t>& key,
    const std::vector<uint8_t>& buffer,
    int& offset) {
  MemCpyFromBuffer(buffer, offset, &key.value_, sizeof(key.value_));
  key.value_ = __builtin_bswap64(key.value_);
  key.value_ ^= cas::kMsbMask64;
}


template<>
void cas::KeyDecoder<std::string>::DecodeValue(
    cas::Key<cas::vstring_t>& key,
    const std::vector<uint8_t>& buffer,
    int& offset) {
  std::size_t size = 0;
  while (buffer[offset+size] != kNullByte) {
    ++size;
  }
  if (size > 0) {
    char value_buffer[size]; // NOLINT
    MemCpyFromBuffer(buffer, offset, value_buffer, size); // NOLINT
    key.value_ = std::string(value_buffer, size); // NOLINT
    ++offset; // skip kNullByte
  }
}

} // namespace cas


// explicit instantiations to separate header from implementation
template class cas::KeyDecoder<cas::vint32_t>;
template class cas::KeyDecoder<cas::vint64_t>;
template class cas::KeyDecoder<cas::vstring_t>;
