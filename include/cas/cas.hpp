#ifndef CAS_CAS_H_
#define CAS_CAS_H_

#include "cas/index.hpp"
#include "cas/key.hpp"
#include "cas/search_key.hpp"
#include "cas/query_stats.hpp"
#include "cas/binary_key.hpp"
#include "cas/index_type.hpp"
#include "cas/node.hpp"
#include <vector>


namespace cas {


template<class VType>
class Cas : public Index<VType> {
public:
  IndexType index_type_;
  Node *root_ = nullptr;
  size_t nr_keys_ = 0;

  Cas(IndexType type);

  ~Cas() override;

  void Insert(Key<VType>& key) override;

  void Insert(BinaryKey& bkey);

  uint64_t BulkLoad(std::deque<Key<VType>>& keys) override;

  uint64_t BulkLoad(std::deque<BinaryKey>& keys);

  const QueryStats QueryRuntime(SearchKey<VType>& key) override;

  const QueryStats Query(SearchKey<VType>& key,
      Emitter<VType> emitter) override;

  const QueryStats Query(SearchKey<VType>& key,
      BinaryKeyEmitter emitter);

  const QueryStats Query(SearchKey<VType>& key,
      RefEmitter emitter);

  void Describe() const override;

  void Dump() const;

  void DumpConcise() const;

  size_t Size() const;

  const cas::IndexStats Stats() const override;

  size_t NrKeys() const override;

private:
  void DeleteNodesRecursively(Node *node);
};


} // namespace cas

#endif // CAS_CAS_H_
