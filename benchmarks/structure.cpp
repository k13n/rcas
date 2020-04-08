#include "benchmark/benchmarks.hpp"


void DatasetSF(std::string dataset_folder) {
  using Exp = benchmark::StructureExperiment<cas::vint64_t>;
  Exp bm(dataset_folder + "/sf_dataset/original/sf_size.csv", ';');
  bm.Run();
}


void DatasetXMark(std::string dataset_folder) {
  using Exp = benchmark::StructureExperiment<cas::vint64_t>;
  Exp bm(dataset_folder + "/xmark/category.csv", ';');
  bm.Run();
}


void DatasetAmazon(std::string dataset_folder) {
  using Exp = benchmark::StructureExperiment<cas::vint32_t>;
  Exp bm(dataset_folder + "/amazon/dataset.csv", ';');
  bm.Run();
}


int main(int argc, char** argv) {
  std::string dataset_folder = "/storage/";
  if (argc >= 2) {
    dataset_folder = argv[1];
  }

  std::cout << "dataset: SF:size" << std::endl;
  DatasetSF(dataset_folder);

  std::cout << "dataset: XMark:category" << std::endl;
  DatasetXMark(dataset_folder);

  std::cout << "dataset: Amazon:price" << std::endl;
  DatasetAmazon(dataset_folder);
  return 0;
}
