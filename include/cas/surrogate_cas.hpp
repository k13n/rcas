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

  void Insert(Key<VType>& key) override;

  uint64_t BulkLoad(std::deque<Key<VType>>& keys) override;

  const QueryStats Query(SearchKey<VType>& key,
      Emitter<VType> emitter) override;

  const QueryStats Query(SearchKey<VType>& key,
      BinaryKeyEmitter emitter);

  const QueryStats QueryRuntime(SearchKey<VType>& key) override;

  const cas::IndexStats Stats() const override;

  size_t NrKeys() const override;

  void Describe() const override;
};


} // namespace cas

#endif // CAS_SURROGATE_CAS_H_
