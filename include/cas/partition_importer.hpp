#ifndef CAS_PARTITION_IMPORTER_H_
#define CAS_PARTITION_IMPORTER_H_

#include "cas/cas.hpp"


namespace cas {

template<class VType>
class PartitionImporter {
  Cas<VType>& index_;

public:
  PartitionImporter(Cas<VType>& index);
  uint64_t BulkLoad(
      const std::string& filename,
      size_t dataset_size,
      size_t page_size
  );

private:
  void CopyFromPage(
      uint8_t* page,
      size_t page_size,
      uint16_t& offset,
      void* dst,
      size_t count);
};

}; // namespace cas

#endif // CAS_PARTITION_IMPORTER_H_
