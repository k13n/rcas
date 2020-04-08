#include "test/catch.hpp"
#include "cas/binary_key.hpp"
#include "cas/node0.hpp"
#include "cas/prefix_matcher.hpp"
#include "comparator.hpp"
#include <iostream>


TEST_CASE("Matching a prefix", "[cas::PrefixMatcher]") {
  cas::BinaryKey bkey1;
  bkey1.path_  = { 0x78, 0x78, 0x78, 0x78 };
  bkey1.value_ = { 0x05, 0x00, 0x03 };
  cas::Node0 node(bkey1, 0, 0);

  cas::BinaryKey bkey2;
  bkey2.path_  = { 0x78, 0x78, 0x79, 0x78 };
  bkey2.value_ = { 0x05, 0x01, 0x04 };

  size_t value_pos = 0;
  size_t path_pos  = 0;
  cas::PrefixMatcher matcher;
  matcher.Match(&node, &bkey2, value_pos, path_pos);

  REQUIRE(matcher.bkey_p_pos_ == 2);
  REQUIRE(matcher.bkey_v_pos_ == 1);
  REQUIRE(matcher.node_p_pos_ == 2);
  REQUIRE(matcher.node_v_pos_ == 5);
  REQUIRE(matcher.path_mismatch_);
  REQUIRE(matcher.value_mismatch_);
}
