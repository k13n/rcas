#ifndef CAS_QUERY_H_
#define CAS_QUERY_H_

#include "cas/node.hpp"
#include "cas/path_matcher.hpp"
#include "cas/query_stats.hpp"
#include "cas/key_encoding.hpp"
#include "cas/search_key.hpp"
#include "cas/index.hpp"

#include <deque>


namespace cas {


template<class VType>
class Query {
  struct State {
    Node* node_;
    Dimension parent_dimension_;
    uint8_t parent_byte_;
    // length of the prefixes matched so far
    uint16_t len_pat_;
    uint16_t len_val_;
    // state needed for the path matching
    PathMatcher::State pm_state_;
    // state needed for the value matching
    uint16_t vl_pos_;
    uint16_t vh_pos_;

    void Dump();
  };

  Node* root_;
  BinarySK& key_;
  PathMatcher& pm_;
  const BinaryKeyEmitter& emitter_;
  std::vector<uint8_t> buf_pat_;
  std::vector<uint8_t> buf_val_;
  std::deque<State> stack_;
  QueryStats stats_;


public:
  Query(Node* root, BinarySK& key, cas::PathMatcher& pm,
      const BinaryKeyEmitter& emitter);

  void Execute();

  const QueryStats& Stats() const {
    return stats_;
  }

private:
  void PrepareBuffer(State& s);

  PathMatcher::PrefixMatch MatchPathPrefix(State& s);

  PathMatcher::PrefixMatch MatchValuePrefix(State& s);

  void Descend(State& s);

  void DescendPathNode(State& s);

  void DescendValueNode(State& s);

  bool IsCompleteValue(State& s);

  void EmitMatch(State& s);

  void UpdateStats(State& s);

  void DumpState(State& s);
};

} // namespace cas

#endif // CAS_QUERY_H_
