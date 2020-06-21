#ifndef CAS_INSERTION_H_
#define CAS_INSERTION_H_

#include "cas/binary_key.hpp"
#include "cas/locator.hpp"
#include "cas/index_type.hpp"

namespace cas {


class Insertion {
protected:
  cas::Node** root_;
  cas::BinaryKey& bkey_;
  cas::Dimension root_dimension_;
  cas::Locator loc_;

public:
  Insertion(cas::Node** root, cas::BinaryKey& bkey,
      cas::Dimension root_dimension = cas::Dimension::Value);

  bool Execute();


private:
  void Locate();

  void AddNewLeaf();

  bool AddRefToExistingLeaf();

  cas::Node* Restructure();

  cas::Dimension DetermineDimension();
};


}; // namespace cas

#endif // CAS_INSERTION_H_
