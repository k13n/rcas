#include "cas/key_encoder.hpp"
#include "cas/key_encoding.hpp"

#include <iostream>
#include <sstream>


template<class VType>
cas::BinaryKey cas::KeyEncoder<VType>::Encode(const cas::Key<VType>& key) {
  BinaryKey bkey;
  ReserveSpace(key, bkey);
  EncodePath(key, bkey);
  EncodeValue(key, bkey);
  bkey.ref_ = key.ref_;
  return bkey;
}


template<class VType>
cas::BinaryKey cas::KeyEncoder<VType>::Encode(
    const cas::Key<VType>& key,
    Surrogate& surrogate) {
  BinaryKey bkey;
  ReserveSpace(key, bkey);
  EncodePath(key, bkey, surrogate);
  EncodeValue(key, bkey);
  bkey.ref_ = key.ref_;
  return bkey;
}


template<class VType>
void
cas::KeyEncoder<VType>::ReserveSpace(const cas::Key<VType>& key, BinaryKey& bkey) {
  size_t path_size = 0;
  for (const auto& label : key.path_) {
    path_size += 1; // for the path separator
    path_size += label.size();
  }
  path_size += 1; // for trailing null byte
  size_t value_size = ValueSize(key.value_);
  bkey.path_  = std::vector<uint8_t>(path_size);
  bkey.value_ = std::vector<uint8_t>(value_size);
}


template<class VType>
void
cas::KeyEncoder<VType>::EncodePath(const cas::Key<VType>& key, BinaryKey& bkey) {
  int offset = 0;
  for (auto& label : key.path_) {
    bkey.path_[offset++] = cas::kPathSep;
    MemCpyToBuffer(bkey.path_, offset, label.data(), label.size());
  }
  bkey.path_[offset++] = cas::kNullByte;
}


template<class VType>
void
cas::KeyEncoder<VType>::EncodePath(const cas::Key<VType>& key, BinaryKey& bkey,
    Surrogate& surrogate) {
  bkey.path_ = surrogate.MapPath(key.path_);
}


template<>
void
cas::KeyEncoder<cas::vint32_t>::EncodeValue(
    const cas::Key<cas::vint32_t>& key,
    BinaryKey& bkey) {
  int offset = 0;
  cas::vint32_t complement = key.value_ ^ cas::kMsbMask32;
  complement = __builtin_bswap32(complement);
  MemCpyToBuffer(bkey.value_, offset, &complement, sizeof(cas::vint32_t));
}


template<>
void
cas::KeyEncoder<cas::vint64_t>::EncodeValue(
    const cas::Key<cas::vint64_t>& key,
    BinaryKey& bkey) {
  int offset = 0;
  cas::vint64_t complement = key.value_ ^ cas::kMsbMask64;
  complement = __builtin_bswap64(complement);
  MemCpyToBuffer(bkey.value_, offset, &complement, sizeof(cas::vint64_t));
}


template<>
void
cas::KeyEncoder<cas::vstring_t>::EncodeValue(
    const cas::Key<cas::vstring_t>& key,
    BinaryKey& bkey) {
  int offset = 0;
  MemCpyToBuffer(bkey.value_, offset, key.value_.data(), key.value_.size());
  bkey.value_[offset++] = cas::kNullByte;
}




template<>
size_t cas::KeyEncoder<cas::vint32_t>::ValueSize(const cas::vint32_t& /*value*/) {
  return sizeof(cas::vint32_t);
}
template<>
size_t cas::KeyEncoder<cas::vint64_t>::ValueSize(const cas::vint64_t& /*value*/) {
  return sizeof(cas::vint64_t);
}
template<>
size_t cas::KeyEncoder<cas::vstring_t>::ValueSize(const cas::vstring_t& value) {
  return value.size() + 1; // for the trailing null byte
}


template<class VType>
cas::BinarySK cas::KeyEncoder<VType>::Encode(cas::SearchKey<VType>& key) {
  cas::BinarySK bkey;
  bkey.low_  = std::vector<uint8_t>(ValueSize(key.low_));
  bkey.high_ = std::vector<uint8_t>(ValueSize(key.high_));
  EncodeValue(key.low_,  bkey.low_);
  EncodeValue(key.high_, bkey.high_);
  EncodeQueryPath(key, bkey);
  return bkey;
}


template<class VType>
cas::BinarySK cas::KeyEncoder<VType>::Encode(
    cas::SearchKey<VType>& key,
    Surrogate& surrogate) {
  cas::BinarySK bkey;
  bkey.low_  = std::vector<uint8_t>(ValueSize(key.low_));
  bkey.high_ = std::vector<uint8_t>(ValueSize(key.high_));
  EncodeValue(key.low_,  bkey.low_);
  EncodeValue(key.high_, bkey.high_);
  EncodeQueryPath(key, bkey, surrogate);
  return bkey;
}


template<>
void
cas::KeyEncoder<cas::vint32_t>::EncodeValue(
    const cas::vint32_t& value,
    std::vector<uint8_t>& buffer) {
  int offset = 0;
  cas::vint32_t complement = value ^ cas::kMsbMask32;
  complement = __builtin_bswap32(complement);
  MemCpyToBuffer(buffer, offset, &complement, sizeof(cas::vint32_t));
}


template<>
void
cas::KeyEncoder<cas::vint64_t>::EncodeValue(
    const cas::vint64_t& value,
    std::vector<uint8_t>& buffer) {
  int offset = 0;
  cas::vint64_t complement = value ^ cas::kMsbMask64;
  complement = __builtin_bswap64(complement);
  MemCpyToBuffer(buffer, offset, &complement, sizeof(cas::vint64_t));
}


template<>
void
cas::KeyEncoder<cas::vstring_t>::EncodeValue(
    const cas::vstring_t& value,
    std::vector<uint8_t>& buffer) {
  int offset = 0;
  MemCpyToBuffer(buffer, offset, value.data(), value.size());
  buffer[offset++] = cas::kNullByte;
}


template<class VType>
void cas::KeyEncoder<VType>::EncodeQueryPath(
    cas::SearchKey<VType>& key,
    cas::BinarySK& bkey) {
  auto& path = key.path_[0];
  bkey.path_.bytes_ = std::vector<uint8_t>(path.size());
  bkey.path_.types_ = std::vector<cas::ByteType>(path.size(),
      cas::ByteType::kTypeLabel);
  for (size_t i = 0; i < path.size(); ++i) {
    if (path[i] == '?') {
      bkey.path_.bytes_[i] = cas::kByteChild;
      bkey.path_.types_[i] = cas::ByteType::kTypeWildcard;
    } else if (path[i] == '^') {
      bkey.path_.bytes_[i] = cas::kByteDescendantOrSelf;
      bkey.path_.types_[i] = cas::ByteType::kTypeDescendant;
    } else if (path[i] == '/') {
      bkey.path_.bytes_[i] = cas::kPathSep;
      bkey.path_.types_[i] = cas::ByteType::kTypePathSeperator;
    } else {
      bkey.path_.bytes_[i] = path[i];
      bkey.path_.types_[i] = cas::ByteType::kTypeLabel;
    }
  }
}


template<class VType>
void cas::KeyEncoder<VType>::EncodeQueryPath(
    cas::SearchKey<VType>& key,
    cas::BinarySK& bkey,
    cas::Surrogate& surrogate) {
  auto& path = key.path_[0];

  std::string label_buffer = "";
  auto flush = [&]() -> void {
    if (label_buffer.empty()) {
      return;
    }
    std::vector<uint8_t> label = surrogate.MapLabel(label_buffer);
    for (size_t i = 0; i < label.size(); ++i) {
      bkey.path_.bytes_.push_back(label[i]);
      bkey.path_.types_.push_back(cas::ByteType::kTypeLabel);
    }
    label_buffer = "";
  };

  for (size_t i = 0; i < path.size(); ++i) {
    if (path[i] == '?') {
      flush();
      bkey.path_.bytes_.push_back(cas::kByteChild);
      bkey.path_.types_.push_back(cas::ByteType::kTypeWildcard);
    } else if (path[i] == '^') {
      flush();
      bkey.path_.bytes_.push_back(cas::kByteDescendantOrSelf);
      bkey.path_.types_.push_back(cas::ByteType::kTypeDescendant);
    } else if (path[i] == '/') {
      // do not translate '/'
      flush();
    } else {
      label_buffer += path[i];
    }
  }
  flush();
}



// explicit instantiations to separate header from implementation
template class cas::KeyEncoder<cas::vint32_t>;
template class cas::KeyEncoder<cas::vint64_t>;
template class cas::KeyEncoder<cas::vstring_t>;
