#ifndef CAS_PATH_MATCHER_H_
#define CAS_PATH_MATCHER_H_

#include "cas/search_key.hpp"
#include "cas/surrogate.hpp"
#include <cstdint>
#include <vector>


namespace cas {

class PathMatcher {
public:
  enum class PrefixMatch {
    MATCH,
    MISMATCH,
    INCOMPLETE,
  };

  struct State {
    uint16_t ppos_ = 0;
    uint16_t qpos_ = 0;
    uint16_t desc_ppos_ = 0;
    int16_t  desc_qpos_ = -1;

    void Dump();
  };


  virtual PrefixMatch MatchPathIncremental(
      const std::vector<uint8_t>& path,
      const cas::BinaryQP& query_path,
      size_t len_path,
      State& state);


  bool MatchPath(
      const std::vector<uint8_t>& path,
      const cas::BinaryQP& query_path);

};


class SurrogatePathMatcher : public PathMatcher {
  Surrogate& surrogate_;

public:
  SurrogatePathMatcher(Surrogate& surrogate);

  virtual PrefixMatch MatchPathIncremental(
      const std::vector<uint8_t>& path,
      const cas::BinaryQP& query_path,
      size_t len_path,
      State& state);
};

}; // namespace cas

#endif // CAS_PATH_MATCHER_H_
