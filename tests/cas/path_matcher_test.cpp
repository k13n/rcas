#include "test/catch.hpp"
#include "cas/key_encoder.hpp"
#include "cas/path_matcher.hpp"
#include "cas/utils.hpp"
#include "comparator.hpp"
#include <iostream>
#include <string>


TEST_CASE("Matching a complete path", "[cas::PathMatcher]") {
  auto match_normal = [&](cas::Key<cas::vint64_t> ikey,
      cas::SearchKey<cas::vint64_t> skey) -> bool {

    cas::KeyEncoder<cas::vint64_t> encoder;
    cas::BinaryKey bikey = encoder.Encode(ikey);
    cas::BinarySK  bskey = encoder.Encode(skey);

    cas::PathMatcher pm;
    return pm.MatchPath(bikey.path_, bskey.path_);
  };


  auto match_surrogate = [&](cas::Key<cas::vint64_t> ikey,
      cas::SearchKey<cas::vint64_t> skey) -> bool {
    int max_depth = 7;
    int bytes_per_label = 2;
    cas::Surrogate surrogate(max_depth, bytes_per_label);

    cas::KeyEncoder<cas::vint64_t> encoder;
    cas::BinaryKey bikey = encoder.Encode(ikey, surrogate);
    cas::BinarySK  bskey = encoder.Encode(skey, surrogate);

    cas::SurrogatePathMatcher pm(surrogate);
    return pm.MatchPath(bikey.path_, bskey.path_);
  };


  auto match = [&](std::string input, std::string pattern) -> bool {
    // build input key with path
    cas::Key<cas::vint64_t> key;
    for (size_t i = 1; i < input.size(); ++i) {
      size_t j = 0;
      while (i+j < input.size() && input[i+j] != '/') {
        ++j;
      }
      key.path_.push_back(input.substr(i, j));
      i += j;
    }

    cas::SearchKey<cas::vint64_t> skey;
    skey.path_ = { pattern };

    return match_normal(key, skey) && match_surrogate(key, skey);
  };

  SECTION("Easy") {
    std::string input =  "/ab/aa/ab/cc/abd";
    REQUIRE(match(input, "/ab/aa/ab/cc/abd"));
    REQUIRE(match(input, "/ab/aa/ac/cc/abd") == false);
    REQUIRE(match(input, "/ab/aa") == false);
  }

  SECTION("Child Wildcard Axis") {
    std::string input =  "/ab/cd/x/a/b";
    REQUIRE(match(input, "/ab/?/x/a/b"));
    REQUIRE(match(input, "/ab/?/x/?/?"));
    REQUIRE(match(input, "/?/?/?/?/?"));
    REQUIRE(match(input, "^ab/?/x^"));
    REQUIRE(match(input, "/?/?^?/?"));
    REQUIRE(match(input, "/?/?/?/?") == false);
    REQUIRE(match(input, "/?/dd/?/?/?") == false);
  }

  SECTION("Descendant") {
    std::string input =  "/ab/aa/ab/cc/abd";
    REQUIRE(match(input, "^ab^abd"));
    REQUIRE(match(input, "/ab^cc/abd"));
    REQUIRE(match(input, "^ab^abd^"));
    REQUIRE(match(input, "^ab/aa^cc/abd"));
    //
    REQUIRE(match(input, "^abc") == false);
    REQUIRE(match(input, "^ab/abd^") == false);
    REQUIRE(match(input, "^ab/aa^cc/ab") == false);
    //
    REQUIRE(match(input, "^abd^^^"));
  }

  SECTION("Match all") {
    std::string pattern = "^";
    REQUIRE(match("/", pattern));
    REQUIRE(match("/a", pattern));
    REQUIRE(match("/a/b", pattern));
    REQUIRE(match("/a/b/c", pattern));
  }

  SECTION("Test1") {
    std::string input = "/ab/abc";
    REQUIRE(match(input, "/ab^abd") == false);
    REQUIRE(match(input, "/ab^?"));
  }

  SECTION("Input too short") {
    std::string pattern = "^abcd";
    REQUIRE(match("/abcd", pattern));
    REQUIRE(match("/abc", pattern) == false);
    REQUIRE(match("/foo/bar/baz/abc", pattern) == false);
    REQUIRE(match("/foo/bar/baz", pattern) == false);
  }

  SECTION("Input too long") {
    std::string pattern = "^abcd";
    REQUIRE(match("/abcd", pattern));
    REQUIRE(match("/abcde", pattern) == false);
    REQUIRE(match("/foo/bar/baz/abcde", pattern) == false);
    REQUIRE(match("/foo/bar/bcde", pattern) == false);
  }

  SECTION("Example") {
    std::string pattern = "/ab/?/ab^ac";
    REQUIRE(match("/ab/aa/ab/cc/ac", pattern));
    REQUIRE(match("/ab/aa/abc", pattern) == false);
    REQUIRE(match("/ab/aa/abd", pattern) == false);
    REQUIRE(match("/ab/aa/ab/cc/abd", pattern) == false);
    REQUIRE(match("/ab/abd", pattern) == false);
    REQUIRE(match("/ab/abd/cc/de/abd", pattern) == false);
    REQUIRE(match("/ab/abd/cc/de/bd", pattern) == false);
  }

  /* SECTION("Special Cases") { */
  /*   REQUIRE(match("/", "/?") == false); */
  /*   REQUIRE(match("/", "^?") == false); */
  /* } */
}
