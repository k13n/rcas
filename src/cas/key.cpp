#include "cas/key.hpp"

#include <iostream>


template<class VType>
cas::Key<VType>::Key(std::string path, VType value, ref_t ref)
    : path_(std::move(path))
    , value_(value)
    , ref_(ref)
{}


template<class VType>
void cas::Key<VType>::Dump() const {
  std::cout <<   "Path:  " << path_;
  std::cout << "\nValue: " << value_;
  std::cout << "\nRef:   " << cas::RefToString(ref_);
}


// explicit instantiations to separate header from implementation
template struct cas::Key<cas::vint32_t>;
template struct cas::Key<cas::vint64_t>;
template struct cas::Key<cas::vstring_t>;
