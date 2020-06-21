#include "benchmark/benchmarks.hpp"


void DatasetSF(const std::string& dataset_folder) {
  using Exp = benchmark::StructureExperiment<cas::vint64_t>;
  Exp bm(dataset_folder + "/sf_dataset/original/sf_size.csv", ';');
  bm.Run();
}


void DatasetXMark(const std::string& dataset_folder) {
  using Exp = benchmark::StructureExperiment<cas::vint64_t>;
  Exp bm(dataset_folder + "/xmark/category.csv", ';');
  bm.Run();
}


void DatasetAmazon(const std::string& dataset_folder) {
  using Exp = benchmark::StructureExperiment<cas::vint32_t>;
  Exp bm(dataset_folder + "/amazon/dataset.csv", ';');
  bm.Run();
}


int main_(int argc, char** argv) {
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


int main(int argc, char** argv) {
  try {
    return main_(argc, argv);
  } catch (std::exception& e) {
    std::cerr << "Standard exception. What: " << e.what() << "\n";
    return 10;
  } catch (...) {
    std::cerr << "Unknown exception.\n";
    return 11;
  }
}
