#include "test/catch.hpp"
#include "cas/interleaver.hpp"
#include "comparator.hpp"


TEST_CASE("Test LabelWise interleaving", "[cas::Interleaver]") {

  SECTION("Path is longer than value") {
    cas::BinaryKey bkey;
    bkey.path_  = { 0xFF, 0x61, 0x62,
                    0xFF, 0x63, 0x64,
                    0xFF, 0x65, 0x66,
                    0xFF, 0x67, 0x00 };
    bkey.value_ = { 0x00, 0x01 };

    std::vector<cas::InterleavedByte> expected_key = {
      { .byte_ = 0xFF, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x61, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x62, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xFF, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x00, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x63, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x64, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xFF, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x01, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x65, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x66, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xFF, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x67, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x00, .dimension_ = cas::Dimension::Path  },
    };

    cas::InterleavedKey ikey = cas::Interleaver::LabelWise(bkey);
    REQUIRE(Comparator::Equals(ikey.bytes_, expected_key));
  }


  SECTION("Value is longer than path") {
    cas::BinaryKey bkey;
    bkey.path_  = { 0xFF, 0x61, 0x62,
                    0xFF, 0x63, 0x64,
                    0xFF, 0x65, 0x66,
                    0xFF, 0x67, 0x00 };
    bkey.value_ = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };

    std::vector<cas::InterleavedByte> expected_key = {
      { .byte_ = 0xFF, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x61, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x62, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xFF, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x00, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x63, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x64, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xFF, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x01, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x65, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x66, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xFF, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x02, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x67, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x00, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x03, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x04, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x05, .dimension_ = cas::Dimension::Value },
    };

    cas::InterleavedKey ikey = cas::Interleaver::LabelWise(bkey);
    REQUIRE(Comparator::Equals(ikey.bytes_, expected_key));
  }
}


TEST_CASE("Test PathValue interleaving", "[cas::Interleaver]") {
  cas::BinaryKey bkey;
  bkey.path_  = { 0x61, 0x62, 0x63, 0x64, 0x65, 0x00 };
  bkey.value_ = { 0x00, 0x01 };

  std::vector<cas::InterleavedByte> expected_key = {
    { .byte_ = 0x61, .dimension_ = cas::Dimension::Path  },
    { .byte_ = 0x62, .dimension_ = cas::Dimension::Path  },
    { .byte_ = 0x63, .dimension_ = cas::Dimension::Path  },
    { .byte_ = 0x64, .dimension_ = cas::Dimension::Path  },
    { .byte_ = 0x65, .dimension_ = cas::Dimension::Path  },
    { .byte_ = 0x00, .dimension_ = cas::Dimension::Path  },
    { .byte_ = 0x00, .dimension_ = cas::Dimension::Value },
    { .byte_ = 0x01, .dimension_ = cas::Dimension::Value },
  };

  cas::InterleavedKey ikey = cas::Interleaver::PathValue(bkey);
  REQUIRE(ikey.ref_ == bkey.ref_);
  REQUIRE(Comparator::Equals(ikey.bytes_, expected_key));
}


TEST_CASE("Test ValuePath interleaving", "[cas::Interleaver]") {
  cas::BinaryKey bkey;
  bkey.path_  = { 0x61, 0x62, 0x63, 0x64, 0x65, 0x00 };
  bkey.value_ = { 0x00, 0x01 };

  std::vector<cas::InterleavedByte> expected_key = {
    { .byte_ = 0x00, .dimension_ = cas::Dimension::Value },
    { .byte_ = 0x01, .dimension_ = cas::Dimension::Value },
    { .byte_ = 0x61, .dimension_ = cas::Dimension::Path  },
    { .byte_ = 0x62, .dimension_ = cas::Dimension::Path  },
    { .byte_ = 0x63, .dimension_ = cas::Dimension::Path  },
    { .byte_ = 0x64, .dimension_ = cas::Dimension::Path  },
    { .byte_ = 0x65, .dimension_ = cas::Dimension::Path  },
    { .byte_ = 0x00, .dimension_ = cas::Dimension::Path  },
  };

  cas::InterleavedKey ikey = cas::Interleaver::ValuePath(bkey);
  REQUIRE(ikey.ref_ == bkey.ref_);
  REQUIRE(Comparator::Equals(ikey.bytes_, expected_key));
}


TEST_CASE("Test ZOrder interleaving", "[cas::Interleaver]") {

  SECTION("Path and value are equally long") {
    cas::BinaryKey bkey;
    bkey.path_  = { 0x01, 0x02, 0x03, 0x04 };
    bkey.value_ = { 0xA1, 0xA2, 0xA3, 0xA4 };

    std::vector<cas::InterleavedByte> expected_key = {
      { .byte_ = 0xA1, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x01, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xA2, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x02, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xA3, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x03, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xA4, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x04, .dimension_ = cas::Dimension::Path  },
    };

    cas::InterleavedKey ikey = cas::Interleaver::ZOrder(bkey);
    REQUIRE(Comparator::Equals(ikey.bytes_, expected_key));
  }


  SECTION("Path is longer than value") {
    cas::BinaryKey bkey;
    bkey.path_  = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    bkey.value_ = { 0xA1, 0xA2, 0xA3, 0xA4 };

    std::vector<cas::InterleavedByte> expected_key = {
      { .byte_ = 0xA1, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x01, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x02, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xA2, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x03, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x04, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xA3, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x05, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xA4, .dimension_ = cas::Dimension::Value },
    };

    cas::InterleavedKey ikey = cas::Interleaver::ZOrder(bkey);
    REQUIRE(Comparator::Equals(ikey.bytes_, expected_key));
  }


  SECTION("Value is longer than path") {
    cas::BinaryKey bkey;
    bkey.path_  = { 0x00, 0x01, 0x02, 0x03 };
    bkey.value_ = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8 };

    std::vector<cas::InterleavedByte> expected_key = {
      { .byte_ = 0xA0, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0xA1, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0xA2, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x00, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xA3, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0xA4, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0xA5, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x01, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0xA6, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0xA7, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0xA8, .dimension_ = cas::Dimension::Value },
      { .byte_ = 0x02, .dimension_ = cas::Dimension::Path  },
      { .byte_ = 0x03, .dimension_ = cas::Dimension::Path  },
    };

    cas::InterleavedKey ikey = cas::Interleaver::ZOrder(bkey);
    REQUIRE(Comparator::Equals(ikey.bytes_, expected_key));
  }

}
