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
  cas::SurrogateCas<cas::vint64_t> index(cas::IndexType::ZOrder, 10, 2);

  /* auto index = GENERATE( */
  /*     std::make_shared<cas::Cas<cas::vint64_t>>(cas::IndexType::DynamicInterleaving), */
  /*     std::make_shared<cas::Cas<cas::vint64_t>>(cas::IndexType::PathValue), */
  /*     std::make_shared<cas::Cas<cas::vint64_t>>(cas::IndexType::ValuePath), */
  /*     std::make_shared<cas::Cas<cas::vint64_t>>(cas::IndexType::LabelWise) */
  /* ); */

  /* auto index = GENERATE( */
  /*     std::make_shared<cas::SurrogateCas<cas::vint64_t>>(cas::IndexType::ZOrder, 10, 2), */
  /*     std::make_shared<cas::SurrogateCas<cas::vint64_t>>(cas::IndexType::DynamicInterleaving, 10, 2), */
  /*     std::make_shared<cas::SurrogateCas<cas::vint64_t>>(cas::IndexType::PathValue, 10, 2), */
  /*     std::make_shared<cas::SurrogateCas<cas::vint64_t>>(cas::IndexType::ValuePath, 10, 2) */
  /* ); */


  cas::vint64_t val = 1;
  cas::Key<cas::vint64_t> key1("/ab/aa/abc", val, 1);
  cas::Key<cas::vint64_t> key2("/ab/aa/abd", val, 2);
  cas::Key<cas::vint64_t> key3("/ab/aa/ab/cc/abd", val, 3);
  cas::Key<cas::vint64_t> key4("/ab/aa/ab/cc/ac", val, 4);
  cas::Key<cas::vint64_t> key5("/ab/abd", val, 5);
  cas::Key<cas::vint64_t> key6("/ab/abd/cc/de/abd", val, 6);
  cas::Key<cas::vint64_t> key7("/ab/abd/cc/de/bd", val, 7);

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
    skey.path_ = "/ab/?/ab^ac";

    vector_key_t<cas::vint64_t> expected = {
      key4
    };
    evaluate(skey, expected);
  }


  SECTION("Query2") {
    cas::SearchKey<cas::vint64_t> skey;
    skey.low_  = val;
    skey.high_ = val;
    skey.path_ = "^";

    vector_key_t<cas::vint64_t> expected = {
      key1, key2, key3, key4, key5, key6, key7
    };
    evaluate(skey, expected);
  }


  SECTION("Query3") {
    cas::SearchKey<cas::vint64_t> skey;
    skey.low_  = val;
    skey.high_ = val;
    skey.path_ = "/ab/aa^";

    vector_key_t<cas::vint64_t> expected = {
      key1, key2, key3, key4
    };
    evaluate(skey, expected);
  }


  SECTION("Query4") {
    cas::SearchKey<cas::vint64_t> skey;
    skey.low_  = val;
    skey.high_ = val;
    skey.path_ = "^abd";

    vector_key_t<cas::vint64_t> expected = {
      key2, key3, key5, key6
    };
    evaluate(skey, expected);
  }
}



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
  /* cas::Key<cas::vint64_t> key1(val, {"ab", "aa", "abc"}, 1); */
  /* cas::Key<cas::vint64_t> key2(val, {"ab", "aa", "abd"}, 2); */
  /* cas::Key<cas::vint64_t> key3(val, {"ab", "aa", "ab", "cc", "abd"}, 3); */
  /* cas::Key<cas::vint64_t> key4(val, {"ab", "aa", "ab", "cc", "ac"}, 4); */
  /* cas::Key<cas::vint64_t> key5(val, {"ab", "abd" }, 5); */
  /* cas::Key<cas::vint64_t> key6(val, {"ab", "abd", "cc", "de", "abd" }, 6); */
  /* cas::Key<cas::vint64_t> key7(val, {"ab", "abd", "cc", "de", "bd" }, 7); */

  cas::Key<cas::vint64_t> key1("/ab/aa/abc", val, 1);
  cas::Key<cas::vint64_t> key2("/ab/aa/abd", val, 2);
  cas::Key<cas::vint64_t> key3("/ab/aa/ab/cc/abd", val, 3);
  cas::Key<cas::vint64_t> key4("/ab/aa/ab/cc/ac", val, 4);
  cas::Key<cas::vint64_t> key5("/ab/abd", val, 5);
  cas::Key<cas::vint64_t> key6("/ab/abd/cc/de/abd", val, 6);
  cas::Key<cas::vint64_t> key7("/ab/abd/cc/de/bd", val, 7);

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
    skey.path_ = "/ab/?/ab^ac";

    vector_key_t<cas::vint64_t> expected = {
      key4
    };
    evaluate(skey, expected);
  }


  SECTION("Query2") {
    cas::SearchKey<cas::vint64_t> skey;
    skey.low_  = val;
    skey.high_ = val;
    skey.path_ = "^";

    vector_key_t<cas::vint64_t> expected = {
      key1, key2, key3, key4, key5, key6, key7
    };
    evaluate(skey, expected);
  }


  SECTION("Query3") {
    cas::SearchKey<cas::vint64_t> skey;
    skey.low_  = val;
    skey.high_ = val;
    skey.path_ = "/ab/aa^";

    vector_key_t<cas::vint64_t> expected = {
      key1, key2, key3, key4
    };
    evaluate(skey, expected);
  }


  SECTION("Query4") {
    cas::SearchKey<cas::vint64_t> skey;
    skey.low_  = val;
    skey.high_ = val;
    skey.path_ = "^abd";

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

  std::string path = "/does/not/matter";
  cas::Key<cas::vstring_t> key1(path, "abbbc", 1);
  cas::Key<cas::vstring_t> key2(path, "abdcbb", 2);
  cas::Key<cas::vstring_t> key3(path, "abdgcd", 3);
  cas::Key<cas::vstring_t> key4(path, "abdgcdgh", 4);
  cas::Key<cas::vstring_t> key5(path, "abdgcdikemx", 5);
  cas::Key<cas::vstring_t> key6(path, "abdgcdikeo", 6);
  cas::Key<cas::vstring_t> key7(path, "abf", 7);

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
    skey.path_ = "^";

    vector_key_t<cas::vstring_t> expected = {
      key3, key4
    };
    evaluate(skey, expected);
  }


  SECTION("Query2") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "abdgcda";
    skey.high_ = "x";
    skey.path_ = "^";

    vector_key_t<cas::vstring_t> expected = {
      key4, key5, key6, key7
    };
    evaluate(skey, expected);
  }


  SECTION("Query3") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "aa";
    skey.high_ = "aba";
    skey.path_ = "^";

    vector_key_t<cas::vstring_t> expected = {
    };
    evaluate(skey, expected);
  }


  SECTION("Query4") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "aa";
    skey.high_ = "x";
    skey.path_ = "^";

    vector_key_t<cas::vstring_t> expected = {
      key1, key2, key3, key4, key5, key6, key7
    };
    evaluate(skey, expected);
  }


  SECTION("Query5") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "aa";
    skey.high_ = "abdd";
    skey.path_ = "^";

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

  cas::Key<cas::vstring_t> key01("/a/a", "uu",  1);
  cas::Key<cas::vstring_t> key02("/a/a", "uv",  2);
  cas::Key<cas::vstring_t> key03("/a/b", "ux",  3);
  cas::Key<cas::vstring_t> key04("/a/b", "uy",  4);
  cas::Key<cas::vstring_t> key05("/a/c", "vu",  5);
  cas::Key<cas::vstring_t> key06("/a/c", "vv",  6);
  cas::Key<cas::vstring_t> key07("/a/d", "vx",  7);
  cas::Key<cas::vstring_t> key08("/a/d", "vy",  8);
  cas::Key<cas::vstring_t> key09("/b/a", "uu",  9);
  cas::Key<cas::vstring_t> key10("/b/a", "uv", 10);
  cas::Key<cas::vstring_t> key11("/b/b", "ux", 11);
  cas::Key<cas::vstring_t> key12("/b/b", "uy", 12);
  cas::Key<cas::vstring_t> key13("/b/c", "vu", 13);
  cas::Key<cas::vstring_t> key14("/b/c", "vv", 14);
  cas::Key<cas::vstring_t> key15("/b/d", "vx", 15);
  cas::Key<cas::vstring_t> key16("/b/d", "vy", 16);
  cas::Key<cas::vstring_t> key17("/c/a", "xu", 17);
  cas::Key<cas::vstring_t> key18("/c/a", "xv", 18);
  cas::Key<cas::vstring_t> key19("/c/b", "xx", 19);
  cas::Key<cas::vstring_t> key20("/c/b", "xy", 20);
  cas::Key<cas::vstring_t> key21("/c/c", "yu", 21);
  cas::Key<cas::vstring_t> key22("/c/c", "yv", 22);
  cas::Key<cas::vstring_t> key23("/c/d", "yx", 23);
  cas::Key<cas::vstring_t> key24("/c/d", "yy", 24);
  cas::Key<cas::vstring_t> key25("/d/a", "xu", 25);
  cas::Key<cas::vstring_t> key26("/d/a", "xv", 26);
  cas::Key<cas::vstring_t> key27("/d/b", "xx", 27);
  cas::Key<cas::vstring_t> key28("/d/b", "xy", 28);
  cas::Key<cas::vstring_t> key29("/d/c", "yu", 29);
  cas::Key<cas::vstring_t> key30("/d/c", "yv", 30);
  cas::Key<cas::vstring_t> key31("/d/d", "yx", 31);
  cas::Key<cas::vstring_t> key32("/d/d", "yy", 32);

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
    skey.path_ = "/b^";

    vector_key_t<cas::vstring_t> expected = {
      key13, key14, key15, key16
    };
    evaluate(skey, expected);
  }


  SECTION("Query2") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "x";
    skey.high_ = "z";
    skey.path_ = "/?/a";

    vector_key_t<cas::vstring_t> expected = {
      key17, key18, key25, key26
    };
    evaluate(skey, expected);
  }


  SECTION("Query3") {
    cas::SearchKey<cas::vstring_t> skey;
    skey.low_  = "y";
    skey.high_ = "yz";
    skey.path_ = "/c^";

    vector_key_t<cas::vstring_t> expected = {
      key21, key22, key23, key24
    };
    evaluate(skey, expected);
  }
}
