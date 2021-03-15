#include "cas/types.hpp"

#include <sstream>
#include <iostream>
#include <iomanip>

std::string cas::RefToString(const cas::ref_t& ref) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (size_t i = 0; i < ref.size(); ++i) {
    ss << std::hex << std::setw(2) << static_cast<int>(ref[i]);
  }
  return ss.str();
}


cas::ref_t cas::RefFromString(const std::string& val) {
  /* return std::stoull(ref); */
  cas::ref_t ref;
  return ref;
}
