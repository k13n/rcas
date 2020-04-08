#include "test/catch.hpp"
#include "cas/key_encoder.hpp"
#include "cas/key_encoding.hpp"
#include "cas/surrogate.hpp"
#include "cas/utils.hpp"
#include "comparator.hpp"

#include <cstdint>


TEST_CASE("Encoding ref in keys", "[cas::KeyEncoder]") {
  cas::Key<cas::vint64_t> key;
  key.ref_ = 123234;

  cas::KeyEncoder<cas::vint64_t> encoder;
  cas::BinaryKey bkey = encoder.Encode(key);

  REQUIRE(bkey.ref_ == key.ref_);
}


TEST_CASE("Encoding path in keys", "[cas::KeyEncoder]") {
  cas::Key<cas::vint64_t> key;
  key.path_ = { "foo", "bar", "baz" };

  cas::KeyEncoder<cas::vint64_t> encoder;
  cas::BinaryKey bkey = encoder.Encode(key);

  std::vector<uint8_t> expected_path = {
    0xFF, 0x66, 0x6F, 0x6F, // /foo
    0xFF, 0x62, 0x61, 0x72, // /bar
    0xFF, 0x62, 0x61, 0x7a, // /baz
    0x00
  };

  REQUIRE(Comparator::Equals(bkey.path_, expected_path));
}


TEST_CASE("Encoding vint64_t values", "[cas::KeyEncoder]") {
  cas::Key<cas::vint64_t> key;
  key.value_ = 1234567890123;

  cas::KeyEncoder<cas::vint64_t> encoder;
  cas::BinaryKey bkey = encoder.Encode(key);

  std::vector<uint8_t> expected_value = {
    0x80, 0x00, 0x01, 0x1F, 0x71, 0xFB, 0x04, 0xCB
  };

  REQUIRE(Comparator::Equals(bkey.value_, expected_value));
}


TEST_CASE("Encoding vint64_t MIN value", "[cas::KeyEncoder]") {
  cas::Key<cas::vint64_t> key;
  key.value_ = INT64_MIN;

  cas::KeyEncoder<cas::vint64_t> encoder;
  cas::BinaryKey bkey = encoder.Encode(key);

  std::vector<uint8_t> expected_value = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  REQUIRE(Comparator::Equals(bkey.value_, expected_value));
}


TEST_CASE("Encoding vint64_t MAX value", "[cas::KeyEncoder]") {
  cas::Key<cas::vint64_t> key;
  key.value_ = INT64_MAX;

  cas::KeyEncoder<cas::vint64_t> encoder;
  cas::BinaryKey bkey = encoder.Encode(key);

  std::vector<uint8_t> expected_value = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
  };

  REQUIRE(Comparator::Equals(bkey.value_, expected_value));
}


TEST_CASE("Encoding vint32_t MIN value", "[cas::KeyEncoder]") {
  cas::Key<cas::vint32_t> key;
  key.value_ = INT32_MIN;

  cas::KeyEncoder<cas::vint32_t> encoder;
  cas::BinaryKey bkey = encoder.Encode(key);

  std::vector<uint8_t> expected_value = {
    0x00, 0x00, 0x00, 0x00
  };

  REQUIRE(Comparator::Equals(bkey.value_, expected_value));
}


TEST_CASE("Encoding vint32_t MAX value", "[cas::KeyEncoder]") {
  cas::Key<cas::vint32_t> key;
  key.value_ = INT32_MAX;

  cas::KeyEncoder<cas::vint32_t> encoder;
  cas::BinaryKey bkey = encoder.Encode(key);

  std::vector<uint8_t> expected_value = {
    0xFF, 0xFF, 0xFF, 0xFF
  };

  REQUIRE(Comparator::Equals(bkey.value_, expected_value));
}


TEST_CASE("Encoding vstring_t values", "[cas::KeyEncoder]") {
  cas::Key<cas::vstring_t> key;
  key.value_ = "Hello!";

  cas::KeyEncoder<cas::vstring_t> encoder;
  cas::BinaryKey bkey = encoder.Encode(key);

  std::vector<uint8_t> expected_value = {
    0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x21, // "Hello!"
    0x00, // trailing NULL byte
  };

  REQUIRE(Comparator::Equals(bkey.value_, expected_value));
}


TEST_CASE("Encoding Query Path with Surrogate", "[cas::KeyEncoder]") {
  cas::SearchKey<cas::vstring_t> key;
  key.low_  = "unimportant";
  key.high_ = "unimportant";
  key.path_ = { "/home^bar/?/baz" };

  size_t max_depth = 6;
  size_t bytes_per_label = 3;
  cas::Surrogate surrogate(max_depth, bytes_per_label);

  std::vector<uint8_t> b1 = surrogate.MapLabel("home");
  std::vector<uint8_t> b2 = surrogate.MapLabel("bar");
  std::vector<uint8_t> b3 = surrogate.MapLabel("baz");

  cas::KeyEncoder<cas::vstring_t> encoder;
  cas::BinarySK skey = encoder.Encode(key, surrogate);

  std::vector<uint8_t> expected_bytes = {
    b1[0], b1[1], b1[2],         // home
    cas::kByteDescendantOrSelf,  // ^
    b2[0], b2[1], b2[2],         // bar
    cas::kByteChild,             // ?
    b3[0], b3[1], b3[2],         // baz
  };

  auto tl = cas::ByteType::kTypeLabel;
  std::vector<cas::ByteType> expected_types = {
    tl, tl, tl,                         // home
    cas::ByteType::kTypeDescendant,     // ^
    tl, tl, tl,                         // bar
    cas::ByteType::kTypeWildcard,       // ?
    tl, tl, tl,                         // baz
  };

  REQUIRE(Comparator::Equals(skey.path_.bytes_, expected_bytes));
  REQUIRE(Comparator::Equals(skey.path_.types_, expected_types));
}
