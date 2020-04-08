#ifndef CASXML_ENTRY_H_
#define CASXML_ENTRY_H_

#include <cstdint>
#include "cas/types.hpp"


namespace casxml {

struct Entry  {
  uint64_t pcr_;
  cas::ref_t ref_;

  void Dump() const;
};

} // namespace casxml

#endif // CASXML_CASXML_H_
