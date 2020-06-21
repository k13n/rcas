#include "benchmark/benchmarks.hpp"


void DatasetSF(const std::string& dataset_folder) {
  using VType = cas::vint64_t;
  using Exp = benchmark::SpaceExperiment<VType>;

  std::vector<benchmark::Approach> approaches = {
    { cas::IndexType::DynamicInterleaving, "dy",  false, 0, 0 },
    { cas::IndexType::ZOrder,              "zo",  true, 21, 3 },
    { cas::IndexType::LabelWise,           "lw",  false, 0, 0 },
    { cas::IndexType::PathValue,           "pv",  false, 0, 0 },
    { cas::IndexType::ValuePath,           "vp",  false, 0, 0 },
    { cas::IndexType::Xml,                 "xml", false, 0, 0 },
  };

  Exp bm(dataset_folder + "/sf_dataset/original/sf_size.csv", ';', approaches);
  bm.Run();
}


void DatasetXMark(const std::string& dataset_folder) {
  using VType = cas::vint32_t;
  using Exp = benchmark::SpaceExperiment<VType>;

  std::vector<benchmark::Approach> approaches = {
    { cas::IndexType::DynamicInterleaving, "dy",  false, 0, 0 },
    { cas::IndexType::ZOrder,              "zo",  true,  5, 1 },
    { cas::IndexType::LabelWise,           "lw",  false, 0, 0 },
    { cas::IndexType::PathValue,           "pv",  false, 0, 0 },
    { cas::IndexType::ValuePath,           "vp",  false, 0, 0 },
    { cas::IndexType::Xml,                 "xml", false, 0, 0 },
  };

  Exp bm(dataset_folder + "/xmark/category.csv", ';', approaches);
  bm.Run();
}


void DatasetAmazon(const std::string& dataset_folder) {
  using VType = cas::vint32_t;
  using Exp = benchmark::SpaceExperiment<VType>;

  std::vector<benchmark::Approach> approaches = {
    { cas::IndexType::DynamicInterleaving, "dy",  false, 0, 0 },
    { cas::IndexType::ZOrder,              "zo",  true, 11, 3 },
    { cas::IndexType::LabelWise,           "lw",  false, 0, 0 },
    { cas::IndexType::PathValue,           "pv",  false, 0, 0 },
    { cas::IndexType::ValuePath,           "vp",  false, 0, 0 },
    { cas::IndexType::Xml,                 "xml", false, 0, 0 },
  };

  Exp bm(dataset_folder + "/amazon/dataset.csv", ';', approaches);
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
