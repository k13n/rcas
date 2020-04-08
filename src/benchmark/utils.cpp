#include "benchmark/utils.hpp"
#include "cas/cas.hpp"
#include "cas/surrogate_cas.hpp"
#include "casxml/casxml.hpp"

template<class VType>
cas::Index<VType>* benchmark::CreateIndex(Approach approach) {
  switch (approach.type_) {
    case cas::IndexType::DynamicInterleaving:
    case cas::IndexType::PathValue:
    case cas::IndexType::ValuePath:
    case cas::IndexType::LabelWise:
      return new cas::Cas<VType>(approach.type_);
    case cas::IndexType::ZOrder:
      return new cas::SurrogateCas<VType>(approach.type_,
          approach.max_depth_, approach.bytes_per_label_);
    case cas::IndexType::Xml:
      return new casxml::CasXml<VType>();
  }
  return nullptr;
}


template cas::Index<cas::vint32_t>* benchmark::CreateIndex<cas::vint32_t>(Approach);
template cas::Index<cas::vint64_t>* benchmark::CreateIndex<cas::vint64_t>(Approach);
template cas::Index<cas::vstring_t>* benchmark::CreateIndex<cas::vstring_t>(Approach);

