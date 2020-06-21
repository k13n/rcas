#ifndef CASXML_CASXML_H_
#define CASXML_CASXML_H_

#include "casxml/entry.hpp"
#include "casxml/allocator.hpp"
#include "cas/cas.hpp"
#include "cas/key.hpp"
#include "cas/search_key.hpp"
#include "cas/index.hpp"
#include "cas/index_type.hpp"
#include <unordered_map>

#include <tlx/container/btree_multimap.hpp>

namespace casxml {

template<class VType>
class CasXml : public cas::Index<VType> {
  /* using BTree = tlx::btree_multimap<VType, Entry>; */
  using BTree = tlx::btree_multimap<
    VType,
    Entry,
    std::less<VType>,
    tlx::btree_default_traits<VType, std::pair<VType, Entry>>,
    casxml::Allocator<std::pair<VType, Entry>>
  >;

  cas::Cas<cas::vstring_t> data_guide_;
  BTree btree_;
  uint64_t max_pcr_ = 0;

  using pcr_map = std::unordered_map<uint64_t, std::vector<std::string>>;
  using btree_iterator = typename BTree::iterator;

public:
  CasXml() :
    data_guide_(cas::IndexType::DynamicInterleaving)
  {}

  ~CasXml() override;

  void Insert(cas::Key<VType>& key) override;

  void Delete(cas::Key<VType>& key);

  uint64_t BulkLoad(std::deque<cas::Key<VType>>& keys) override;

  const cas::QueryStats Query(cas::SearchKey<VType>& key,
      cas::Emitter<VType> emitter) override;

  const cas::QueryStats Query(cas::SearchKey<VType>& key,
      bool decode, cas::Emitter<VType> emitter);

  const cas::QueryStats QueryRuntime(cas::SearchKey<VType>& key) override;

  void Dump();

  void Describe() const override;

  const cas::IndexStats Stats() const override;

  size_t NrKeys() const override;

private:
  uint64_t ComputePcr(cas::Key<VType>& key);

  const pcr_map QueryDataGuide(cas::SearchKey<VType>& key,
      cas::QueryStats& stats, bool decode);

  std::string CasType() const;
};

} // namespace casxml

#endif // CASXML_CASXML_H_
