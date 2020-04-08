#include "compile.hpp"
#include "cas/cas.hpp"
#include "cas/csv_importer.hpp"
#include "benchmark/selectivity_computer.hpp"


void PaperRunningExample() {
  cas::Cas<cas::vint64_t> index(cas::IndexType::DynamicInterleaving);
  std::string dataset = "../datasets/bom.csv";
  cas::CsvImporter<cas::vint64_t> importer(index, '\t');
  importer.BulkLoad(dataset);
  index.Describe();
  index.DumpConcise();
}


void SelectivityRepl() {
  /* using VType = cas::vstring_t; */
  using VType = cas::vint64_t;
  /* std::string dataset = "/storage/costmodel/full.txt"; */
  /* std::string dataset = "/storage/costmodel/full_changing_prefix_len.txt"; */
  /* std::string dataset = "/storage/costmodel/full_integer_keys1.txt"; */
  /* std::string dataset = "/storage/costmodel/full_integer_keys5.txt"; */
  /* std::string dataset = "/storage/costmodel/full_integer_keys6.txt"; */
  /* std::string dataset = "/storage/costmodel/full-16-3_2.txt"; */
  /* std::string dataset = "/storage/costmodel/full-8-8_1.txt"; */
  /* std::string dataset = "/storage/costmodel/full-8-8_2.deleted31p.txt"; */
  /* std::string dataset = "/storage/product_catalog/dataset.csv"; */
  std::string dataset = "/storage/product_catalog/dataset.csv";

  cas::Cas<VType> index(cas::IndexType::DynamicInterleaving);
  cas::CsvImporter<VType> importer(index, ';');
  importer.BulkLoad(dataset);
  index.Describe();

  benchmark::SelectivityComputer<VType> c(index);
  while (true) {
    /* std::cin >> std::hex; */
    std::string path;
    VType low, high;
    std::cout << "Query Path: ";
    std::cin >> path;
    std::cout << "Low:  ";
    std::cin >> low;
    std::cout << "High: ";
    std::cin >> high;
    std::cout << std::endl;
    c.Compute(path, low, high);
    std::cout << std::endl;
    std::cout << std::endl;
  }
}

int main(int /*argc*/, char** /*argv*/) {
  /* SelectivityRepl(); */
  PaperRunningExample();
  return 0;
}
