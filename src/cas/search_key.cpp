#include "cas/search_key.hpp"
#include "cas/utils.hpp"

#include <iostream>


template<class VType>
void cas::SearchKey<VType>::Dump() const {
  std::cout << "SearchKey";
  std::cout << "\nPath: " << path_;
  std::cout << "\nLow:  " << low_;
  std::cout << "\nHigh: " << high_;
  std::cout << "\n";
}


template<class VType>
void cas::SearchKey<VType>::DumpConcise() const {
  std::cout << "Q(";
  std::cout << path_;
  std::cout << ", " << low_;
  std::cout << ", " << high_;
  std::cout << ")\n";
}


void cas::BinarySK::Dump() const {
  std::cout << "SearchKey(Binary)";
  std::cout << "\nPath: ";
  Utils::DumpHexValues(path_);
  std::cout << "\nLow:  ";
  Utils::DumpHexValues(low_);
  std::cout << "\nHigh: ";
  Utils::DumpHexValues(high_);
  std::cout << "\n";
}


// explicit instantiations to separate header from implementation
template struct cas::SearchKey<cas::vint32_t>;
template struct cas::SearchKey<cas::vint64_t>;
template struct cas::SearchKey<cas::vstring_t>;
