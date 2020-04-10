#include "compile.hpp"
#include "cas/cas.hpp"
#include "cas/csv_importer.hpp"


int main(int /*argc*/, char** /*argv*/) {
  cas::Cas<cas::vint64_t> index(cas::IndexType::DynamicInterleaving);
  std::string dataset = "../datasets/bom.csv";
  cas::CsvImporter<cas::vint64_t> importer(index, '\t');
  importer.BulkLoad(dataset);
  index.Describe();
  index.DumpConcise();
  return 0;
}
