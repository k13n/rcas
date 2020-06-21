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

  Cas(const Cas<VType>& other) = delete;
  Cas(const Cas<VType>&& other) = delete;
  Cas<VType>& operator=(const Cas<VType>& other) = delete;
  Cas<VType>& operator=(Cas<VType>&& other) = delete;

  void Insert(Key<VType>& key) override;

  void Insert(BinaryKey& bkey);

  uint64_t BulkLoad(std::deque<Key<VType>>& keys) override;

  uint64_t BulkLoad(std::deque<BinaryKey>& keys);

  QueryStats QueryRuntime(SearchKey<VType>& key) override;

  QueryStats Query(SearchKey<VType>& key,
      const Emitter<VType>& emitter) override;

  QueryStats Query(SearchKey<VType>& key,
      const BinaryKeyEmitter& emitter);

  QueryStats Query(SearchKey<VType>& key,
      const RefEmitter& emitter);

  void Describe() const override;

  void Dump() const;

  void DumpConcise() const;

  size_t Size() const;

  cas::IndexStats Stats() const override;

  size_t NrKeys() const override;

private:
  void DeleteNodesRecursively(Node *node);
};


} // namespace cas

#endif // CAS_CAS_H_
