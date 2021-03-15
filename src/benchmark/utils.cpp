#include "benchmark/utils.hpp"
#include "cas/cas.hpp"
#include "cas/surrogate_cas.hpp"
/* #include "casxml/casxml.hpp" */

template<class VType>
std::unique_ptr<cas::Index<VType>> benchmark::CreateIndex(Approach approach) {
  switch (approach.type_) {
    case cas::IndexType::DynamicInterleaving:
    case cas::IndexType::PathValue:
    case cas::IndexType::ValuePath:
    case cas::IndexType::LabelWise:
      return std::make_unique<cas::Cas<VType>>(approach.type_);
    case cas::IndexType::ZOrder:
      return std::make_unique<cas::SurrogateCas<VType>>(approach.type_,
          approach.max_depth_, approach.bytes_per_label_);
    /* case cas::IndexType::Xml: */
    /*   return std::make_unique<casxml::CasXml<VType>>(); */
  }
  return nullptr;
}


template std::unique_ptr<cas::Index<cas::vint32_t>> benchmark::CreateIndex<cas::vint32_t>(Approach);
template std::unique_ptr<cas::Index<cas::vint64_t>> benchmark::CreateIndex<cas::vint64_t>(Approach);
template std::unique_ptr<cas::Index<cas::vstring_t>> benchmark::CreateIndex<cas::vstring_t>(Approach);

