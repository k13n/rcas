#include "cas/search_key.hpp"
#include "cas/utils.hpp"

#include <iostream>


template<class VType>
void cas::SearchKey<VType>::Dump() const {
  std::cout << "SearchKey" << std::endl;
  std::cout << "Low:  " << low_ << std::endl;
  std::cout << "High: " << high_ << std::endl;
  std::cout << "QueryPath: ";
  for (auto& v : path_) {
    std::cout << "/" << v;
  }
  std::cout << std::endl;
}


template<class VType>
void cas::SearchKey<VType>::DumpConcise() const {
  std::cout << "Q(";
  std::cout << path_[0];
  std::cout << ", " << low_;
  std::cout << ", " << high_;
  std::cout << ")";
  std::cout << std::endl;
}


void cas::BinarySK::Dump() const {
  std::cout << "SearchKey(Binary)" << std::endl;
  std::cout << "Path: ";
  Utils::DumpHexValues(path_);
  std::cout << "\nLow:  ";
  Utils::DumpHexValues(low_);
  std::cout << "\nHigh: ";
  Utils::DumpHexValues(high_);
}


// explicit instantiations to separate header from implementation
template struct cas::SearchKey<cas::vint32_t>;
template struct cas::SearchKey<cas::vint64_t>;
template struct cas::SearchKey<cas::vstring_t>;
