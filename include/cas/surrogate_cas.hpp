#ifndef CAS_SURROGATE_CAS_H_
#define CAS_SURROGATE_CAS_H_

#include "cas/cas.hpp"
#include "cas/surrogate.hpp"
#include <vector>
#include <optional>


namespace cas {


template<class VType>
class SurrogateCas : public Index<VType> {
private:
  Cas<VType> index_;
  Surrogate surrogate_;

public:
  SurrogateCas(IndexType type, size_t s_max_depth, size_t s_bytes_per_label);

  ~SurrogateCas();

  void Insert(Key<VType>& key);

  uint64_t BulkLoad(std::deque<Key<VType>>& keys);

  const QueryStats Query(SearchKey<VType>& key,
      BinaryKeyEmitter emitter);

  const QueryStats Query(SearchKey<VType>& key,
      Emitter<VType> emitter);

  const QueryStats QueryRuntime(SearchKey<VType>& key);

  const cas::IndexStats Stats() const;

  size_t NrKeys() const;

  void Describe() const;
};


} // namespace cas

#endif // CAS_SURROGATE_CAS_H_
