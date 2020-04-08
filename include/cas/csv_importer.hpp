#ifndef CAS_CSV_IMPORTER_H_
#define CAS_CSV_IMPORTER_H_

#include "cas/index.hpp"
#include "cas/key.hpp"


namespace cas {

template<class VType>
class CsvImporter {
  Index<VType>& index_;
  ref_t highest_ref_;
  char delimiter_;

public:
  CsvImporter(Index<VType>& index, char delimiter_ = ' ');

  void Load(std::string filename);

  uint64_t BulkLoad(std::string filename);

  const cas::Key<VType> ProcessLine(const std::string& line);

private:
  VType ParseValue(std::string& value);
};

}; // namespace cas

#endif // CAS_CSV_IMPORTER_H_


