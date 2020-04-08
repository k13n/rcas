#include "cas/search_key.hpp"
#include "cas/utils.hpp"

#include <iostream>


template<class VType>
void cas::SearchKey<VType>::Dump() {
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
void cas::SearchKey<VType>::DumpConcise() {
  std::cout << "Q(";
  std::cout << path_[0];
  std::cout << ", " << low_;
  std::cout << ", " << high_;
  std::cout << ")";
  std::cout << std::endl;
}


void cas::BinarySK::Dump() {
  std::cout << "SearchKey(Binary)" << std::endl;
  std::cout << "Low:  ";
  Utils::DumpHexValues(low_);
  std::cout << std::endl;
  std::cout << "High: ";
  Utils::DumpHexValues(high_);
  std::cout << std::endl;
  path_.Dump();
}


size_t cas::BinaryQP::Size() const {
  return bytes_.size();
}


void cas::BinaryQP::Dump() const {
  std::cout << "QueryPath: ";
  Utils::DumpHexValues(bytes_);
  std::cout << std::endl;
  std::cout << "PathTypes: ";
  for (const cas::ByteType& type : types_) {
    std::cout << "   " << type << " ";
  }
  std::cout << std::endl;
}


// explicit instantiations to separate header from implementation
template struct cas::SearchKey<cas::vint32_t>;
template struct cas::SearchKey<cas::vint64_t>;
template struct cas::SearchKey<cas::vstring_t>;
