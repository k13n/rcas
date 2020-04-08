#include "cas/key.hpp"
#include "cas/interleaved_key.hpp"
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>


template<class VType>
/* using vector_key_t = std::vector<std::reference_wrapper<cas::Key<VType>>>; */
using vector_key_t = std::vector<cas::Key<VType>>;


class Comparator {

public:

  template<class VType>
  static bool Equals(
      const std::vector<VType>& v1,
      const std::vector<VType>& v2) {
    if (v1.size() != v1.size()) {
      return false;
    }
    for (size_t i = 0; i < v1.size(); ++i) {
      if (v1[i] != v2[i]) {
        return false;
      }
    }
    return true;
  }


  template<class VType>
  static bool Equals(
      vector_key_t<VType>& set1,
      vector_key_t<VType>& set2) {
    if (set1.size() != set2.size()) {
      return false;
    }
    for (cas::Key<VType>& key1 : set1) {
      bool found = false;
      for (cas::Key<VType>& key2 : set2) {
        if (key1 == key2) {
          found = true;
          break;
        }
      }
      if (!found) {
        return false;
      }
    }
    return true;
  }


  static bool Equals(
      const std::vector<cas::InterleavedByte>& bytes1,
      const std::vector<cas::InterleavedByte>& bytes2) {
    if (bytes1.size() != bytes2.size()) {
      return false;
    }
    for (size_t i = 0; i < bytes1.size(); ++i) {
      if (bytes1[i].byte_ != bytes2[i].byte_ ||
          bytes1[i].dimension_ != bytes2[i].dimension_) {
        return false;
      }
    }
    return true;
  }

};
