#include "test/catch.hpp"
#include "cas/insertion.hpp"
#include "cas/binary_key.hpp"
#include "cas/node0.hpp"
#include "comparator.hpp"
#include <iostream>


TEST_CASE("Insertion in Path Root Node", "[cas::Insertion]") {
  cas::BinaryKey bkey1;
  bkey1.path_  = { 0x61, 0x62, 0x63, 0x64, 0x65 };
  bkey1.value_ = { 0x05, 0x00, 0x03 };
  bkey1.ref_   = 2;

  cas::BinaryKey bkey2;
  bkey2.path_  = { 0x61, 0x62, 0x63, 0x78, 0x64, 0x65 };
  bkey2.value_ = { 0x05, 0x01, 0x04 };
  bkey2.ref_   = 2;

  cas::Node* root = nullptr;
  cas::Insertion insertion1(&root, bkey1, cas::Dimension::Value);
  insertion1.Execute();
  cas::Insertion insertion2(&root, bkey2, cas::Dimension::Value);
  insertion2.Execute();

  std::vector<uint8_t> expected_parent_prefix = {
    0x61, 0x62, 0x63, // from the path
    0x05,             // from the value
  };
  std::vector<uint8_t> expected_node_prefix = {
    0x64, 0x65,       // from the path
    0x03,             // from the value
  };
  std::vector<uint8_t> expected_sibling_prefix = {
    0x78, 0x64, 0x65, // from the path
    0x04,             // from the value
  };

  REQUIRE(root->Type() == cas::Dimension::Value);
  REQUIRE(root->separator_pos_ == 3);
  REQUIRE(Comparator::Equals(root->prefix_, expected_parent_prefix));

  cas::Node* old_child = root->LocateChild(0x00);
  cas::Node* new_child = root->LocateChild(0x01);

  REQUIRE(old_child != nullptr);
  REQUIRE(new_child != nullptr);

  REQUIRE(old_child->Type() == cas::Dimension::Leaf);
  REQUIRE(old_child->separator_pos_ == 2);
  REQUIRE(Comparator::Equals(old_child->prefix_, expected_node_prefix));
  REQUIRE(static_cast<cas::Node0*>(old_child)->ContainsRef(bkey1.ref_));

  REQUIRE(new_child->Type() == cas::Dimension::Leaf);
  REQUIRE(new_child->separator_pos_ == 3);
  REQUIRE(Comparator::Equals(new_child->prefix_, expected_sibling_prefix));
  REQUIRE(static_cast<cas::Node0*>(new_child)->ContainsRef(bkey2.ref_));
}


TEST_CASE("Insertion in Value Root Node", "[cas::Insertion]") {
  cas::BinaryKey bkey1;
  bkey1.path_  = { 0x61, 0x62, 0x63, 0x64, 0x65 };
  bkey1.value_ = { 0x05, 0x00, 0x03 };
  bkey1.ref_   = 1;

  cas::BinaryKey bkey2;
  bkey2.path_  = { 0x61, 0x62, 0x63, 0x78, 0x64, 0x65 };
  bkey2.value_ = { 0x05, 0x01, 0x04 };
  bkey2.ref_   = 2;

  cas::Node* root = nullptr;
  cas::Insertion insertion1(&root, bkey1, cas::Dimension::Path);
  insertion1.Execute();
  cas::Insertion insertion2(&root, bkey2, cas::Dimension::Path);
  insertion2.Execute();

  std::vector<uint8_t> expected_parent_prefix = {
    0x61, 0x62, 0x63, // from the path
    0x05,             // from the value
  };
  std::vector<uint8_t> expected_node_prefix = {
    0x65,             // from the path
    0x00, 0x03        // from the value
  };
  std::vector<uint8_t> expected_sibling_prefix = {
    0x64, 0x65,       // from the path
    0x01, 0x04,       // from the value
  };

  REQUIRE(root->Type() == cas::Dimension::Path);
  REQUIRE(root->separator_pos_ == 3);
  REQUIRE(Comparator::Equals(root->prefix_, expected_parent_prefix));

  cas::Node* old_child = root->LocateChild(0x64);
  cas::Node* new_child = root->LocateChild(0x78);

  REQUIRE(old_child != nullptr);
  REQUIRE(new_child != nullptr);

  REQUIRE(old_child->Type() == cas::Dimension::Leaf);
  REQUIRE(old_child->separator_pos_ == 1);
  REQUIRE(Comparator::Equals(old_child->prefix_, expected_node_prefix));
  REQUIRE(static_cast<cas::Node0*>(old_child)->ContainsRef(bkey1.ref_));

  REQUIRE(new_child->Type() == cas::Dimension::Leaf);
  REQUIRE(new_child->separator_pos_ == 2);
  REQUIRE(Comparator::Equals(new_child->prefix_, expected_sibling_prefix));
  REQUIRE(static_cast<cas::Node0*>(new_child)->ContainsRef(bkey2.ref_));
}


TEST_CASE("Insertion of new Leaf in inner node", "[cas::Insertion]") {
  cas::BinaryKey bkey1;
  bkey1.path_  = { 0x61, 0x62, 0x63, 0x64, 0x65 };
  bkey1.value_ = { 0x05, 0x00, 0x03 };
  bkey1.ref_   = 1;

  cas::BinaryKey bkey2;
  bkey2.path_  = { 0x61, 0x62, 0x63, 0x78, 0x64, 0x65 };
  bkey2.value_ = { 0x05, 0x01, 0x04 };
  bkey2.ref_   = 2;

  cas::BinaryKey bkey3;
  bkey3.path_  = { 0x61, 0x62, 0x63, 0x65 };
  bkey3.value_ = { 0x05, 0x00, 0x03 };
  bkey3.ref_   = 3;

  cas::Node* root = nullptr;
  cas::Insertion insertion1(&root, bkey1, cas::Dimension::Path);
  insertion1.Execute();
  cas::Insertion insertion2(&root, bkey2, cas::Dimension::Path);
  insertion2.Execute();
  cas::Insertion insertion3(&root, bkey3, cas::Dimension::Path);
  insertion3.Execute();

  std::vector<uint8_t> expected_leaf_prefix = {
    0x00, 0x03,  // from the value
  };

  cas::Node* new_leaf = root->LocateChild(0x65);
  REQUIRE(new_leaf != nullptr);
  REQUIRE(new_leaf->Type() == cas::Dimension::Leaf);
  REQUIRE(new_leaf->separator_pos_ == 0);
  REQUIRE(Comparator::Equals(new_leaf->prefix_, expected_leaf_prefix));
  REQUIRE(static_cast<cas::Node0*>(new_leaf)->ContainsRef(bkey3.ref_));
}


TEST_CASE("Insertion may grow a node", "[cas::Insertion]") {
  cas::Node* root = nullptr;

  for (int i = 4; i >= 0; --i) {
    uint8_t byte = static_cast<uint8_t>(i);
    cas::BinaryKey bkey;
    bkey.path_  = { 0x61, 0x62, byte, 0x64, 0x65 };
    bkey.value_ = { 0x05, 0x00, 0x03 };
    bkey.ref_   = i;
    cas::Insertion insertion(&root, bkey, cas::Dimension::Path);
    insertion.Execute();
  }

  REQUIRE(root != nullptr);
  REQUIRE(root->nr_children_ == 5);
  for (int i = 4; i >= 0; --i) {
    uint8_t byte = static_cast<uint8_t>(i);
    REQUIRE(root->LocateChild(byte) != nullptr);
  }
}


TEST_CASE("Insertion in a multi-level tree", "[cas::Insertion]") {
  cas::Node* root = nullptr;

  cas::BinaryKey bkey1;
  bkey1.path_  = { 0x61, 0x62, 0x63, 0x64, 0x65 };
  bkey1.value_ = { 0x05, 0x00, 0x03 };
  bkey1.ref_   = 1;

  cas::BinaryKey bkey2;
  bkey2.path_  = { 0x61, 0x62, 0x63, 0x78, 0x64, 0x65 };
  bkey2.value_ = { 0x05, 0x01, 0x04 };
  bkey2.ref_   = 2;

  cas::BinaryKey bkey3;
  bkey3.path_  = { 0x62, 0x63, 0x64, 0x65 };
  bkey3.value_ = { 0x05, 0x01, 0x04 };
  bkey3.ref_   = 3;

  cas::BinaryKey bkey4;
  bkey4.path_  = { 0x61, 0x62, 0x63, 0x78, 0x65 };
  bkey4.value_ = { 0x05, 0x01, 0x04 };
  bkey4.ref_   = 4;

  cas::Insertion insertion1(&root, bkey1, cas::Dimension::Value);
  insertion1.Execute();
  cas::Insertion insertion2(&root, bkey2, cas::Dimension::Value);
  insertion2.Execute();
  cas::Insertion insertion3(&root, bkey3, cas::Dimension::Value);
  insertion3.Execute();
  cas::Insertion insertion4(&root, bkey4, cas::Dimension::Value);
  insertion4.Execute();

  std::vector<uint8_t> expected_prefix_n1 = {
    0x05,             // from the value
  };
  std::vector<uint8_t> expected_prefix_n2 = {
    0x62, 0x63,       // from the path
  };
  std::vector<uint8_t> expected_prefix_n3 = {
    0x63, 0x64, 0x65, // from the path
    0x01, 0x04,       // from the value
  };
  std::vector<uint8_t> expected_prefix_n4 = {
    0x64, 0x65,       // from the path
    0x03              // from the value
  };
  std::vector<uint8_t> expected_prefix_n5 = {
    0x78,             // from the path
    0x04,             // from the value
  };
  std::vector<uint8_t> expected_prefix_n6 = {
    0x65,             // from the path
  };
  std::vector<uint8_t> expected_prefix_n7 = {
  };

  REQUIRE(root != nullptr);
  REQUIRE(root->Type() == cas::Dimension::Path);
  REQUIRE(root->separator_pos_ == 0);
  REQUIRE(Comparator::Equals(root->prefix_, expected_prefix_n1));

  cas::Node* n2 = root->LocateChild(0x61);
  REQUIRE(n2 != nullptr);
  REQUIRE(n2->Type() == cas::Dimension::Value);
  REQUIRE(n2->separator_pos_ == 2);
  REQUIRE(Comparator::Equals(n2->prefix_, expected_prefix_n2));

  cas::Node* n3 = root->LocateChild(0x62);
  REQUIRE(n3 != nullptr);
  REQUIRE(n3->Type() == cas::Dimension::Leaf);
  REQUIRE(n3->separator_pos_ == 3);
  REQUIRE(Comparator::Equals(n3->prefix_, expected_prefix_n3));

  cas::Node* n4 = n2->LocateChild(0x00);
  REQUIRE(n4 != nullptr);
  REQUIRE(n4->Type() == cas::Dimension::Leaf);
  REQUIRE(n4->separator_pos_ == 2);
  REQUIRE(Comparator::Equals(n4->prefix_, expected_prefix_n4));

  cas::Node* n5 = n2->LocateChild(0x01);
  REQUIRE(n5 != nullptr);
  REQUIRE(n5->Type() == cas::Dimension::Path);
  REQUIRE(n5->separator_pos_ == 1);
  REQUIRE(Comparator::Equals(n5->prefix_, expected_prefix_n5));

  cas::Node* n6 = n5->LocateChild(0x64);
  REQUIRE(n6 != nullptr);
  REQUIRE(n6->Type() == cas::Dimension::Leaf);
  REQUIRE(n6->separator_pos_ == 1);
  REQUIRE(Comparator::Equals(n6->prefix_, expected_prefix_n6));

  cas::Node* n7 = n5->LocateChild(0x65);
  REQUIRE(n7 != nullptr);
  REQUIRE(n7->Type() == cas::Dimension::Leaf);
  REQUIRE(n7->separator_pos_ == 0);
  REQUIRE(Comparator::Equals(n7->prefix_, expected_prefix_n7));
}



TEST_CASE("Add ref to existing leaf", "[cas::Insertion]") {
  cas::BinaryKey bkey1;
  bkey1.path_  = { 0x61, 0x62, 0x63, 0x64, 0x65 };
  bkey1.value_ = { 0x05, 0x00, 0x03 };
  bkey1.ref_   = 1;

  cas::BinaryKey bkey2;
  bkey2.path_  = { 0x61, 0x62, 0x63, 0x64, 0x65 };
  bkey2.value_ = { 0x05, 0x00, 0x03 };
  bkey2.ref_   = 2;

  cas::Node* root = nullptr;
  cas::Insertion insertion1(&root, bkey1, cas::Dimension::Value);
  insertion1.Execute();
  cas::Insertion insertion2(&root, bkey2, cas::Dimension::Value);
  insertion2.Execute();

  REQUIRE(root != nullptr);
  REQUIRE(root->IsLeaf());
  REQUIRE(static_cast<cas::Node0*>(root)->ContainsRef(bkey1.ref_));
  REQUIRE(static_cast<cas::Node0*>(root)->ContainsRef(bkey2.ref_));
}


TEST_CASE("Insertion of new Leaf in inner node that has same color as children",
    "[cas::Insertion]") {
  cas::BinaryKey bkey1;
  bkey1.path_  = { 0x61, 0x62, 0x63, 0x64, 0x65 };
  bkey1.value_ = { 0x05, 0x00, 0x03 };
  bkey1.ref_   = 1;

  cas::BinaryKey bkey2;
  bkey2.path_  = { 0x61, 0x62, 0x78, 0x64 };
  bkey2.value_ = { 0x05, 0x00, 0x03 };
  bkey2.ref_   = 2;

  cas::BinaryKey bkey3;
  bkey3.path_  = { 0x61, 0x62, 0x64, 0x65, 0x66 };
  bkey3.value_ = { 0x05, 0x00, 0x03 };
  bkey3.ref_   = 3;

  cas::Node0 node(bkey1, 0, 0);
  cas::Node* root = &node;

  cas::Insertion insertion1(&root, bkey2, cas::Dimension::Value);
  insertion1.Execute();
  cas::Insertion insertion2(&root, bkey3, cas::Dimension::Value);
  insertion2.Execute();


  cas::Node* n1 = root;
  REQUIRE(n1 != nullptr);
  REQUIRE(n1->Type() == cas::Dimension::Path);

  cas::Node* n2 = root->LocateChild(0x63);
  REQUIRE(n2 != nullptr);
  REQUIRE(n2->Type() == cas::Dimension::Leaf);

  cas::Node* n3 = root->LocateChild(0x64);
  REQUIRE(n3 != nullptr);
  REQUIRE(n3->Type() == cas::Dimension::Leaf);

  cas::Node* n4 = root->LocateChild(0x78);
  REQUIRE(n4 != nullptr);
  REQUIRE(n4->Type() == cas::Dimension::Leaf);
}
