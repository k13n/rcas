#include "cas/surrogate.hpp"
#include <iostream>


cas::Surrogate::Surrogate(size_t max_depth, size_t bytes_per_label)
  : max_depth_(max_depth)
  , bytes_per_label_(bytes_per_label)
{ }


std::vector<uint8_t> cas::Surrogate::MapLabel(const std::string& label) {
  uint32_t surrogate;
  auto it = map_.find(label);
  if (it == map_.end()) {
    surrogate = ++counter_;
    map_[label] = surrogate;
    map_inv_[surrogate] = label;
  } else {
    surrogate = it->second;
  }

  int pos = 0;
  std::vector<uint8_t> bytes(bytes_per_label_, 0x00);
  for (int i = bytes_per_label_-1; i >= 0; --i) {
    // extract the i-th byte from the surrogate
    bytes[pos] = (surrogate >> (i * 8)) & 0xFF;
    ++pos;
  }

  return bytes;
};


std::vector<uint8_t> cas::Surrogate::MapPath(const std::vector<std::string>& path) {
  if (path.size() > max_depth_) {
    return std::vector<uint8_t>();
  }
  std::vector<uint8_t> bytes;
  for (const auto& label : path) {
    std::vector<uint8_t> surrogate = MapLabel(label);
    bytes.insert(bytes.end(), surrogate.begin(), surrogate.end());
  }
  for (size_t i = bytes.size(); i < NrBytes(); ++i) {
    bytes.push_back(0x00);
  }
  return bytes;
}


size_t cas::Surrogate::NrBytes() const {
  return max_depth_ * bytes_per_label_;
}


std::string cas::Surrogate::MapLabelInv(const std::vector<uint8_t>& bytes) {
  uint32_t surrogate = 0;
  for (size_t i = 0; i < bytes_per_label_; ++i) {
    int pos = bytes_per_label_ - i - 1;
    surrogate |= static_cast<uint32_t>(bytes[i]) << (pos*8);
  }
  auto it = map_inv_.find(surrogate);
  if (it == map_inv_.end()) {
    return "";
  } else {
    return it->second;
  }
}
