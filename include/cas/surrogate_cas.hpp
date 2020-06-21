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
  ~SurrogateCas() = default;

  SurrogateCas(const SurrogateCas<VType>& other) = delete;
  SurrogateCas(const SurrogateCas<VType>&& other) = delete;
  SurrogateCas<VType>& operator=(const SurrogateCas<VType>& other) = delete;
  SurrogateCas<VType>& operator=(SurrogateCas<VType>&& other) = delete;

  void Insert(Key<VType>& key) override;

  uint64_t BulkLoad(std::deque<Key<VType>>& keys) override;

  QueryStats Query(SearchKey<VType>& key,
      const Emitter<VType>& emitter) override;

  QueryStats Query(SearchKey<VType>& key,
      const BinaryKeyEmitter& emitter);

  QueryStats QueryRuntime(SearchKey<VType>& key) override;

  cas::IndexStats Stats() const override;

  size_t NrKeys() const override;

  void Describe() const override;
};


} // namespace cas

#endif // CAS_SURROGATE_CAS_H_
