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
  EncodeQueryPath(key, bkey.path_);
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
    std::vector<uint8_t>& buffer) {
  buffer.reserve(key.path_.size());
  for (char symbol : key.path_) {
    if (symbol == '?') {
      buffer.push_back(static_cast<uint8_t>(cas::PathMask::Wildcard));
    } else if (symbol == '^') {
      buffer.push_back(static_cast<uint8_t>(cas::PathMask::Descendant));
    } else if (symbol == '/') {
      buffer.push_back(static_cast<uint8_t>(cas::PathMask::PathSeperator));
    } else {
      buffer.push_back(static_cast<uint8_t>(symbol));
    }
  }
}


template<class VType>
void cas::KeyEncoder<VType>::EncodeQueryPath(
    cas::SearchKey<VType>& key,
    cas::BinarySK& skey,
    cas::Surrogate& surrogate) {
  std::string label_buffer = "";
  auto flush = [&]() -> void {
    if (label_buffer.empty()) {
      return;
    }
    std::vector<uint8_t> label = surrogate.MapLabel(label_buffer);
    for (size_t i = 0; i < label.size(); ++i) {
      skey.path_.push_back(label[i]);
      skey.mask_.push_back(cas::PathMask::Label);
    }
    label_buffer = "";
  };

  for (size_t i = 0; i < key.path_.size(); ++i) {
    if (key.path_[i] == '?') {
      flush();
      skey.path_.push_back(static_cast<uint8_t>(cas::PathMask::Wildcard));
      skey.mask_.push_back(cas::PathMask::Wildcard);
    } else if (key.path_[i] == '^') {
      flush();
      skey.path_.push_back(static_cast<uint8_t>(cas::PathMask::Descendant));
      skey.mask_.push_back(cas::PathMask::Descendant);
    } else if (key.path_[i] == '/') {
      // do not translate '/'
      flush();
    } else {
      label_buffer += key.path_[i];
    }
  }
  flush();
}



// explicit instantiations to separate header from implementation
template class cas::KeyEncoder<cas::vint32_t>;
template class cas::KeyEncoder<cas::vint64_t>;
template class cas::KeyEncoder<cas::vstring_t>;
