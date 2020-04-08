#include "cas/binary_key.hpp"

#include "cas/utils.hpp"
#include <iostream>


const std::vector<uint8_t>& cas::BinaryKey::Get(cas::Dimension dimension) {
  switch (dimension) {
  case cas::Dimension::Path:
    return path_;
  case cas::Dimension::Value:
    return value_;
  default:
    throw std::runtime_error("invalid");
  }
}

void cas::BinaryKey::Dump() const {
  printf("Path  (%2lu): ", path_.size());
  cas::Utils::DumpHexValues(path_);
  std::cout << std::endl;
  printf("Value (%2lu): ", value_.size());
  cas::Utils::DumpHexValues(value_);
  std::cout << std::endl;
  std::cout << "Ref:        " <<  ref_;
  std::cout << std::endl;
}
