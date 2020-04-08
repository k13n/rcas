#include "test/catch.hpp"
#include "cas/surrogate.hpp"
#include "cas/utils.hpp"
#include "comparator.hpp"


TEST_CASE("MapLabel", "[cas::Surrogate]") {
  int max_depth = 5;
  int bytes_per_label = 3;

  cas::Surrogate sg(max_depth, bytes_per_label);
  std::vector<uint8_t> v01 = sg.MapLabel("foo");
  std::vector<uint8_t> v02 = sg.MapLabel("barr");
  std::vector<uint8_t> v03 = sg.MapLabel("baz");

  REQUIRE(Comparator::Equals(v01, sg.MapLabel("foo")));
  REQUIRE(Comparator::Equals(v02, sg.MapLabel("barr")));
  REQUIRE(Comparator::Equals(v03, sg.MapLabel("baz")));

  REQUIRE(!Comparator::Equals(v01, v02));
  REQUIRE(!Comparator::Equals(v01, v03));
  REQUIRE(!Comparator::Equals(v02, v03));
}


TEST_CASE("MapLabelInv", "[cas::Surrogate]") {
  int max_depth = 5;
  int bytes_per_label = 3;

  cas::Surrogate sg(max_depth, bytes_per_label);
  std::vector<uint8_t> v01 = sg.MapLabel("foo");
  std::vector<uint8_t> v02 = sg.MapLabel("barr");
  std::vector<uint8_t> v03 = sg.MapLabel("baz");

  std::vector<uint8_t> e01 = {0x00, 0x00, 0x01};
  std::vector<uint8_t> e02 = {0x00, 0x00, 0x02};
  std::vector<uint8_t> e03 = {0x00, 0x00, 0x03};

  REQUIRE(Comparator::Equals(v01, e01));
  REQUIRE(Comparator::Equals(v02, e02));
  REQUIRE(Comparator::Equals(v03, e03));

  REQUIRE(sg.MapLabelInv(v01) == "foo");
  REQUIRE(sg.MapLabelInv(v02) == "barr");
  REQUIRE(sg.MapLabelInv(v03) == "baz");
}


TEST_CASE("MapPath", "[cas::Surrogate]") {
  int max_depth = 5;
  int bytes_per_label = 3;

  cas::Surrogate sg(max_depth, bytes_per_label);
  std::vector<std::string> path = { "foo", "barr", "baz" };
  std::vector<uint8_t> spath = sg.MapPath(path);

  REQUIRE(spath.size() == max_depth * bytes_per_label);
  REQUIRE(sg.NrBytes() == max_depth * bytes_per_label);

  std::vector<uint8_t> v01 = sg.MapLabel(path[0]);
  std::vector<uint8_t> v02 = sg.MapLabel(path[1]);
  std::vector<uint8_t> v03 = sg.MapLabel(path[2]);

  REQUIRE(Comparator::Equals(v01, {0x00, 0x00, 0x01}));
  REQUIRE(Comparator::Equals(v02, {0x00, 0x00, 0x02}));
  REQUIRE(Comparator::Equals(v03, {0x00, 0x00, 0x03}));

  std::vector<uint8_t> expected_spath = {
    0x00, 0x00, 0x01, // foo
    0x00, 0x00, 0x02, // barr
    0x00, 0x00, 0x03, // baz
    0x00, 0x00, 0x00, // (padding)
    0x00, 0x00, 0x00, // (padding)
  };

  REQUIRE(Comparator::Equals(spath, expected_spath));
}
