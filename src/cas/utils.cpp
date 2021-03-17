#include "cas/utils.hpp"
#include "cas/key_encoding.hpp"
#include "cas/types.hpp"

#include <cstring>
#include <iostream>
#include <sstream>
#include <regex>
#include <iomanip>
#include <chrono>


void cas::Utils::DumpHexValues(const std::vector<uint8_t>& buffer) {
  DumpHexValues(buffer, 0, buffer.size());
}


void cas::Utils::DumpHexValues(const std::vector<uint8_t>& buffer, size_t size) {
  DumpHexValues(buffer, 0, size);
}


void cas::Utils::DumpHexValues(const std::vector<uint8_t>& buffer,
    size_t offset, size_t size) {
  for (size_t i = offset; i < size && i < buffer.size(); ++i) {
    printf("0x%02X", (unsigned char) buffer[i]); // NOLINT
    if (i < buffer.size()-1) {
      printf(" "); // NOLINT
    }
  }
}


void cas::Utils::DumpChars(std::vector<uint8_t>& buffer) {
  DumpChars(buffer, 0, buffer.size());
}


void cas::Utils::DumpChars(std::vector<uint8_t>& buffer, size_t size) {
  DumpChars(buffer, 0, size);
}


void cas::Utils::DumpChars(std::vector<uint8_t>& buffer, size_t offset, size_t size) {
  DumpChars(buffer, offset, size, std::cout);
}


void cas::Utils::DumpChars(std::vector<uint8_t>& buffer, size_t offset,
                           size_t size, std::ostream& os) {
  for (size_t i = offset; i < size && i < buffer.size(); ++i) {
    uint8_t symbol = buffer[i];
    if (symbol == cas::kPathSep) {
      symbol = '/';
    }
    if (symbol == cas::kNullByte) {
      os << '\\';
      symbol = '$';
    }
    os << static_cast<unsigned char>(symbol);
  }
}


int cas::Utils::Memcmp(const void* lhs, size_t len_lhs, const void* rhs, size_t len_rhs) {
  size_t length = std::min<size_t>(len_lhs, len_rhs);
  int cmp = std::memcmp(lhs, rhs, length);
  if (cmp != 0) {
    // one is already lexicographically before or after the other
    return cmp;
  }
  if (len_lhs == len_rhs) {
    // lhs and rhs are equal
    return 0;
  }
  // one string is a prefix of the other. the shorter orders before the other
  return len_lhs < len_rhs ? -1 : 1;
}


template<>
std::string cas::Utils::TypeToString<cas::vint32_t>() {
  return "int32_t";
}
template<>
std::string cas::Utils::TypeToString<cas::vint64_t>() {
  return "int64_t";
}
template<>
std::string cas::Utils::TypeToString<cas::vstring_t>() {
  return "std::string";
}


void cas::Utils::Log(const std::string& msg) {
  std::cout << "[" << cas::Utils::CurrentIsoTime() << "] " << msg;
}


std::string cas::Utils::CurrentIsoTime() {
  auto now = std::chrono::system_clock::now();
  auto stime = std::chrono::system_clock::to_time_t(now);
  auto ltime = std::localtime(&stime);
  std::stringstream stream;
  stream << std::put_time(ltime, "%FT%T%z");
  return stream.str();
}
