#include "test/catch.hpp"
#include "cas/cas.hpp"
#include "cas/surrogate_cas.hpp"
#include "cas/search_key.hpp"
#include "cas/utils.hpp"
#include "casxml/casxml.hpp"
#include "comparator.hpp"
#include <iostream>
#include <functional>


TEST_CASE("Test Path Only Queries", "[cas::Query]") {
  /* auto index = GENERATE( */
  /*     cas::Cas<cas::vint64_t>(cas::IndexType::DynamicInterleaving), */
  /*     cas::Cas<cas::vint64_t>(cas::IndexType::PathValue), */
  /*     cas::Cas<cas::vint64_t>(cas::IndexType::ValuePath), */
  /*     cas::Cas<cas::vint64_t>(cas::IndexType::LabelWise) */
  /* ); */
  auto index = GENERATE(
      cas::SurrogateCas<cas::vint64_t>(cas::IndexType::ZOrder, 10, 2),
      cas::SurrogateCas<cas::vint64_t>(cas::IndexType::DynamicInterleaving, 10, 2),
      cas::SurrogateCas<cas::vint64_t>(cas::IndexType::PathValue, 10, 2),
      cas::SurrogateCas<cas::vint64_t>(cas::IndexType::ValuePath, 10, 2)
  );


  cas::vint64_t val = 1;
  cas::Key<cas::vint64_t> key1(val, {"ab", "aa", "abc"}, 1);
  cas::Key<cas::vint64_t> key2(val, {"ab", "aa", "abd"}, 2);
  cas::Key<cas::vint64_t> key3(val, {"ab", "aa", "ab", "cc", "abd"}, 3);
  cas::Key<cas::vint64_t> key4(val, {"ab", "aa", "ab", "cc", "ac"}, 4);
  cas::Key<cas::vint64_t> key5(val, {"ab", "abd" }, 5);
  cas::Key<cas::vint64_t> key6(val, {"ab", "abd", "cc", "de", "abd" }, 6);
  cas::Key<cas::vint64_t> key7(val, {"ab", "abd", "cc", "de", "bd" }, 7);

  index.Insert(key1);
  index.Insert(key2);
  index.Insert(key3);
  index.Insert(key4);
  index.Insert(key5);
  index.Insert(key6);
  index.Insert(key7);

  auto evaluate = [&](cas::SearchKey<cas::vint64_t>& skey,
                      vector_key_t<cas::vint64_t>& expected) -> void {
    vector_key_t<cas::vint64_t> result;
    index.Query(skey, [&](cas::Key<cas::vint64_t> match) -> void {
      result.push_back(match);
    });
    REQUIRE(Comparator::Equals(expected, result));
  };

  SECTION("Query1") {
    cas::SearchKey<cas::vint64_t> skey;
    skey.low_  = val;
    skey.high_ = val;
    skey.path_ = { "/ab/?/ab^ac" };

    vector_key_t<cas::vint64_t> expected = {
      key4
    };
    evaluate(skey, expected);
  }


  SECTION("Query2") {
    cas::SearchKey<cas::vint64_t> skey;
    skey.low_  = val;
    skey.high_ = val;
    skey.path_ = { "^" };

    vector_key_t<cas::vint64_t> expected = {
      key1, key2, key3, key4, key5, key6, key7
    };
    evaluate(skey, expected);
  }


  SECTION("Query3") {
    cas::SearchKey<cas::vint64_t> skey;
    skey.low_  = val;
    skey.high_ = val;
    skey.path_ = { "/ab/aa^" };

    vector_key_t<cas::vint64_t> expected = {
      key1, key2, key3, key4
    };
    evaluate(skey, expected);
  }


  SECTION("Query4") {
    cas::SearchKey<cas::vint64_t> skey;
    skey.low_  = val;
    skey.high_ = val;
    skey.path_ = { "^abd" };

    vector_key_t<cas::vint64_t> expected = {
      key2, key3, key5, key6
    };
    evaluate(skey, expected);
  }
}



TEST_CASE("Test Value Only Queries", "[cas::Query]") {
  auto index = GENERATE(
      cas::Cas<cas::vstring_t>(cas::IndexType::DynamicInterleaving),
      cas::Cas<cas::vstring_t>(cas::IndexType::PathValue),
      cas::Cas<cas::vstring_t>(cas::IndexType::ValuePath),
      cas::Cas<cas::vstring_t>(cas::IndexType::LabelWise)
  );
  /* auto index = GENERATE( */
  /*     cas::SurrogateCas<cas::vstring_t>(cas::IndexType::ZOrder, 10, 2), */
  /*     cas::SurrogateCas<cas::vstring_t>(cas::IndexType::DynamicInterleaving, 10, 2), */
  /*     cas::SurrogateCas<cas::vstring_t>(cas::IndexType::PathValue, 10, 2), */
  /*     cas::SurrogateCas<cas::vstring_t>(cas::IndexType::ValuePath, 10, 2) */
  /* ); */

  std::vector<cas::vstring_t> path = { "does", "not", "matter" };
  cas::Key<cas::vstring_t> key1("abbbc", path, 1);
  cas::Key<cas::vstring_t> key2("abdcbb", path, 2);
  cas::Key<cas::vstring_t> key3("abdgcd", path, 3);
  cas::Key<cas::vstring_t> key4("abdgcdgh", path, 4);
  cas::Key<cas::vstring_t> key5("abdgcdikemx", path, 5);
  cas::Key<cas::vstring_t> key6("abdgcdikeo", path, 6);
  cas::Key<cas::vstring_t> key7("abf", path, 7);

  index.Insert(key1);
  index.Insert(key2);
  index.Insert(key3);
  index.Insert(key4);
  index.Insert(key5);
  index.Insert(key6);
  index.Insert(key7);

  auto evaluate = [&](cas::SearchKey<cas::vstring_t>& skey,
                      vector_key_t<cas::vstring_t>& expected) -> void {
    vector_key_t<cas::vstring_t> result;
    index.Query(skey, [&](cas::Key<cas::vstring_t> match) -> void {
      result.push_back(match);
    });
    REQUIRE(Comparator::Equals(expected, result));
  };


  SECTION("Query1") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "abdd";
    skey.high_ = "abdgcdh";
    skey.path_ = { "^" };

    vector_key_t<cas::vstring_t> expected = {
      key3, key4
    };
    evaluate(skey, expected);
  }


  SECTION("Query2") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "abdgcda";
    skey.high_ = "x";
    skey.path_ = { "^" };

    vector_key_t<cas::vstring_t> expected = {
      key4, key5, key6, key7
    };
    evaluate(skey, expected);
  }


  SECTION("Query3") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "aa";
    skey.high_ = "aba";
    skey.path_ = { "^" };

    vector_key_t<cas::vstring_t> expected = {
    };
    evaluate(skey, expected);
  }


  SECTION("Query4") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "aa";
    skey.high_ = "x";
    skey.path_ = { "^" };

    vector_key_t<cas::vstring_t> expected = {
      key1, key2, key3, key4, key5, key6, key7
    };
    evaluate(skey, expected);
  }


  SECTION("Query5") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "aa";
    skey.high_ = "abdd";
    skey.path_ = { "^" };

    vector_key_t<cas::vstring_t> expected = {
      key1, key2
    };
    evaluate(skey, expected);
  }
}



TEST_CASE("Test Mixed Queries", "[cas::Query]") {
  auto index = GENERATE(
      cas::Cas<cas::vstring_t>(cas::IndexType::DynamicInterleaving),
      cas::Cas<cas::vstring_t>(cas::IndexType::PathValue),
      cas::Cas<cas::vstring_t>(cas::IndexType::ValuePath),
      cas::Cas<cas::vstring_t>(cas::IndexType::LabelWise)
  );
  /* auto index = GENERATE( */
  /*     cas::SurrogateCas<cas::vstring_t>(cas::IndexType::ZOrder, 10, 2), */
  /*     cas::SurrogateCas<cas::vstring_t>(cas::IndexType::DynamicInterleaving, 10, 2), */
  /*     cas::SurrogateCas<cas::vstring_t>(cas::IndexType::PathValue, 10, 2), */
  /*     cas::SurrogateCas<cas::vstring_t>(cas::IndexType::ValuePath, 10, 2) */
  /* ); */

  cas::Key<cas::vstring_t> key01("uu", {"a", "a"},  1);
  cas::Key<cas::vstring_t> key02("uv", {"a", "a"},  2);
  cas::Key<cas::vstring_t> key03("ux", {"a", "b"},  3);
  cas::Key<cas::vstring_t> key04("uy", {"a", "b"},  4);
  cas::Key<cas::vstring_t> key05("vu", {"a", "c"},  5);
  cas::Key<cas::vstring_t> key06("vv", {"a", "c"},  6);
  cas::Key<cas::vstring_t> key07("vx", {"a", "d"},  7);
  cas::Key<cas::vstring_t> key08("vy", {"a", "d"},  8);
  cas::Key<cas::vstring_t> key09("uu", {"b", "a"},  9);
  cas::Key<cas::vstring_t> key10("uv", {"b", "a"}, 10);
  cas::Key<cas::vstring_t> key11("ux", {"b", "b"}, 11);
  cas::Key<cas::vstring_t> key12("uy", {"b", "b"}, 12);
  cas::Key<cas::vstring_t> key13("vu", {"b", "c"}, 13);
  cas::Key<cas::vstring_t> key14("vv", {"b", "c"}, 14);
  cas::Key<cas::vstring_t> key15("vx", {"b", "d"}, 15);
  cas::Key<cas::vstring_t> key16("vy", {"b", "d"}, 16);
  cas::Key<cas::vstring_t> key17("xu", {"c", "a"}, 17);
  cas::Key<cas::vstring_t> key18("xv", {"c", "a"}, 18);
  cas::Key<cas::vstring_t> key19("xx", {"c", "b"}, 19);
  cas::Key<cas::vstring_t> key20("xy", {"c", "b"}, 20);
  cas::Key<cas::vstring_t> key21("yu", {"c", "c"}, 21);
  cas::Key<cas::vstring_t> key22("yv", {"c", "c"}, 22);
  cas::Key<cas::vstring_t> key23("yx", {"c", "d"}, 23);
  cas::Key<cas::vstring_t> key24("yy", {"c", "d"}, 24);
  cas::Key<cas::vstring_t> key25("xu", {"d", "a"}, 25);
  cas::Key<cas::vstring_t> key26("xv", {"d", "a"}, 26);
  cas::Key<cas::vstring_t> key27("xx", {"d", "b"}, 27);
  cas::Key<cas::vstring_t> key28("xy", {"d", "b"}, 28);
  cas::Key<cas::vstring_t> key29("yu", {"d", "c"}, 29);
  cas::Key<cas::vstring_t> key30("yv", {"d", "c"}, 30);
  cas::Key<cas::vstring_t> key31("yx", {"d", "d"}, 31);
  cas::Key<cas::vstring_t> key32("yy", {"d", "d"}, 32);

  index.Insert(key01);
  index.Insert(key02);
  index.Insert(key03);
  index.Insert(key04);
  index.Insert(key05);
  index.Insert(key06);
  index.Insert(key07);
  index.Insert(key08);
  index.Insert(key09);
  index.Insert(key10);
  index.Insert(key11);
  index.Insert(key12);
  index.Insert(key13);
  index.Insert(key14);
  index.Insert(key15);
  index.Insert(key16);
  index.Insert(key17);
  index.Insert(key18);
  index.Insert(key19);
  index.Insert(key20);
  index.Insert(key21);
  index.Insert(key22);
  index.Insert(key23);
  index.Insert(key24);
  index.Insert(key25);
  index.Insert(key26);
  index.Insert(key27);
  index.Insert(key28);
  index.Insert(key29);
  index.Insert(key30);
  index.Insert(key31);
  index.Insert(key32);

  auto evaluate = [&](cas::SearchKey<cas::vstring_t>& skey,
                      vector_key_t<cas::vstring_t>& expected) -> void {
    vector_key_t<cas::vstring_t> result;
    index.Query(skey, [&](cas::Key<cas::vstring_t> match) -> void {
      result.push_back(match);
    });
    REQUIRE(Comparator::Equals(expected, result));
  };


  SECTION("Query1") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "v";
    skey.high_ = "xz";
    skey.path_ = { "/b^" };

    vector_key_t<cas::vstring_t> expected = {
      key13, key14, key15, key16
    };
    evaluate(skey, expected);
  }


  SECTION("Query2") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "x";
    skey.high_ = "z";
    skey.path_ = { "/?/a" };

    vector_key_t<cas::vstring_t> expected = {
      key17, key18, key25, key26
    };
    evaluate(skey, expected);
  }


  SECTION("Query3") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "y";
    skey.high_ = "yz";
    skey.path_ = { "/c^" };

    vector_key_t<cas::vstring_t> expected = {
      key21, key22, key23, key24
    };
    evaluate(skey, expected);
  }
}
