#ifndef CAS_INDEX_H_
#define CAS_INDEX_H_

#include "cas/key.hpp"
#include "cas/search_key.hpp"
#include "cas/query_stats.hpp"
#include "cas/index_stats.hpp"

#include <functional>
#include <deque>

namespace cas {


template<class VType>
using Emitter = std::function<void(const Key<VType>&)>;

using RefEmitter = std::function<void(ref_t)>;

using BinaryEmitter = std::function<void(const std::vector<uint8_t>&)>;

using BinaryKeyEmitter = std::function<void(
    const std::vector<uint8_t>& buffer_path,
    const std::vector<uint8_t>& buffer_value,
    ref_t ref)>;


template<class VType>
class Index {
public:
  Index() {}

  virtual ~Index();

  virtual void Insert(Key<VType>& key) = 0;

  virtual uint64_t BulkLoad(std::deque<Key<VType>>& keys) = 0;

  virtual const QueryStats Query(SearchKey<VType>& key,
      Emitter<VType> emitter) = 0;

  virtual const QueryStats QueryRuntime(
      SearchKey<VType>& key) = 0;

  const QueryStats QueryVerbose(SearchKey<VType>& key);

  virtual const IndexStats Stats() const = 0;

  virtual size_t NrKeys() const = 0;

  virtual void Describe() const = 0;
};


} // namespace cas

#endif // CAS_INDEX_H_
