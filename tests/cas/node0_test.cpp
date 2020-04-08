#include "test/catch.hpp"
#include "cas/node0.hpp"
#include "cas/binary_key.hpp"
#include "comparator.hpp"


TEST_CASE("Node0 Constructor", "[cas::Node0]") {
  cas::BinaryKey bkey;
  bkey.value_ = { 0x09, 0x08, 0x07, 0x06 };
  bkey.path_  = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
  bkey.ref_   = 29384;

  size_t value_pos = 3;
  size_t path_pos  = 2;

  std::vector<uint8_t> expected_prefix_ = {
    0x03, 0x04, 0x05, 0x06, // from the path
    0x06, // from the value
  };

  cas::Node0 node(bkey, path_pos, value_pos);

  REQUIRE(std::find(node.refs_.begin(), node.refs_.end(), bkey.ref_) !=
      node.refs_.end());
  REQUIRE(node.separator_pos_ == 4);
  REQUIRE(node.Type() == cas::Dimension::Leaf);
  REQUIRE(Comparator::Equals(node.prefix_, expected_prefix_));
}
