#ifndef CAS_CSV_IMPORTER_H_
#define CAS_CSV_IMPORTER_H_

#include "cas/index.hpp"
#include "cas/key.hpp"


namespace cas {

template<class VType>
class CsvImporter {
  Index<VType>& index_;
  char delimiter_;
  ref_t highest_ref_;

public:
  CsvImporter(Index<VType>& index, char delimiter_ = ' ');

  void Load(const std::string& filename);

  uint64_t BulkLoad(const std::string& filename);

  cas::Key<VType> ProcessLine(const std::string& line);

private:
  VType ParseValue(std::string& value);
};

}; // namespace cas

#endif // CAS_CSV_IMPORTER_H_


