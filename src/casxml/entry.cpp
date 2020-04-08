#include "casxml/entry.hpp"

#include <iostream>


void casxml::Entry::Dump() const {
  std::cout << "PCR: " << pcr_ << std::endl;
  std::cout << "Ref: " << ref_ << std::endl;
}
