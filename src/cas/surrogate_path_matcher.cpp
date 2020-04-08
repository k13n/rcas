#include "cas/path_matcher.hpp"
#include "cas/key_encoding.hpp"
#include "cas/search_key.hpp"
#include "cas/utils.hpp"
#include <iostream>
#include <cassert>


cas::SurrogatePathMatcher::SurrogatePathMatcher(cas::Surrogate& surrogate)
  : surrogate_(surrogate)
{}


cas::SurrogatePathMatcher::PrefixMatch cas::SurrogatePathMatcher::MatchPathIncremental(
    const std::vector<uint8_t>& path,
    const cas::BinaryQP& qpath,
    size_t len_path,
    State& s) {

  /* cas::Utils::DumpHexValues(path, len_path); */
  /* std::cout << std::endl; */
  /* cas::Utils::DumpHexValues(qpath.bytes_); */
  /* std::cout << std::endl; */
  /* std::cout << std::endl; */
  /* s.Dump(); std::cout << std::endl; */

  while (s.ppos_ < len_path && s.desc_ppos_ < len_path) {
    if (s.qpos_ < qpath.Size()
        && qpath.types_[s.qpos_] == cas::ByteType::kTypeLabel
        && path[s.ppos_] == qpath.bytes_[s.qpos_]) {
      // simple pattern symbol matches input symbol
      ++s.ppos_;
      ++s.qpos_;
    } else if (s.qpos_ < qpath.Size()
        && qpath.types_[s.qpos_] == cas::ByteType::kTypeWildcard) {
      // child axis is fully matched when the next label begins
      ++s.ppos_;
      if (s.ppos_ % surrogate_.bytes_per_label_ == 0) {
        ++s.qpos_;
      }
    } else if (s.qpos_ < qpath.Size()
        && qpath.types_[s.qpos_] == cas::ByteType::kTypeDescendant
        && s.ppos_ % surrogate_.bytes_per_label_ == 0) {
      // remember were we found the last descendant-or-self axis
      s.desc_ppos_ = s.ppos_;
      s.desc_qpos_ = s.qpos_;
      ++s.qpos_;
    } else if (s.qpos_ == qpath.Size() && path[s.ppos_] == 0x00) {
      // match trailing 0x00s padded by the surrogate_
      ++s.ppos_;
    } else if (s.desc_qpos_ != -1) {
      // the descendant-or-self axis must match a larger substring
      if ((s.desc_ppos_+1) % surrogate_.bytes_per_label_ == 0) {
        s.ppos_ = s.desc_ppos_ + 1;
        s.qpos_ = s.desc_qpos_ + 1;
      }
      ++s.desc_ppos_;
    } else {
      return MISMATCH;
    }
  }

  // we need more input characters to determine the outcome
  if (s.ppos_ < surrogate_.NrBytes() && s.desc_ppos_ < surrogate_.NrBytes()) {
    return INCOMPLETE;
  }

  return (s.qpos_ == qpath.Size()) ? MATCH : MISMATCH;
}
