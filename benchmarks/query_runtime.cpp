#include "benchmark/benchmarks.hpp"


void DatasetSF(std::string dataset_folder) {
  using VType = cas::vint64_t;
  using Exp = benchmark::QueryExperiment<VType>;

  std::vector<benchmark::Approach> approaches = {
    { cas::IndexType::DynamicInterleaving, "dy",  false, 0, 0 },
    { cas::IndexType::ZOrder,              "zo",  true, 21, 3 },
    { cas::IndexType::LabelWise,           "lw",  false, 0, 0 },
    { cas::IndexType::PathValue,           "pv",  false, 0, 0 },
    { cas::IndexType::ValuePath,           "vp",  false, 0, 0 },
    { cas::IndexType::Xml,                 "xml", false, 0, 0 },
  };

  std::vector<cas::SearchKey<VType>> queries = {
    Query<VType>("/usr/include^", 5000, cas::VINT64_MAX),
    Query<VType>("/usr/include^", 3000, 4000),
    Query<VType>("/usr/lib^", 0, 1000),
    Query<VType>("/usr/share^Makefile", 1000, 2000),
    Query<VType>("/usr/share/doc^README", 4000, 5000),
    Query<VType>("/etc^", 5000, cas::VINT64_MAX),
  };

  Exp bm(dataset_folder + "/sf_dataset/original/sf_size.csv", ';',
      approaches, queries);
  bm.Run();
}


void DatasetXMark(std::string dataset_folder) {
  using VType = cas::vint32_t;
  using Exp = benchmark::QueryExperiment<VType>;

  std::vector<benchmark::Approach> approaches = {
    { cas::IndexType::DynamicInterleaving, "dy",  false, 0, 0 },
    { cas::IndexType::ZOrder,              "zo",  true,  5, 1 },
    { cas::IndexType::LabelWise,           "lw",  false, 0, 0 },
    { cas::IndexType::PathValue,           "pv",  false, 0, 0 },
    { cas::IndexType::ValuePath,           "vp",  false, 0, 0 },
    { cas::IndexType::Xml,                 "xml", false, 0, 0 },
  };

  std::vector<cas::SearchKey<VType>> queries = {
    Query<VType>("/site/people^interest", 0,  50000),
    Query<VType>("/site/regions/africa^", 0,  50000),
  };

  Exp bm(dataset_folder + "/xmark/category.csv", ';',
      approaches, queries);
  bm.Run();
}


void DatasetAmazon(std::string dataset_folder) {
  using VType = cas::vint32_t;
  using Exp = benchmark::QueryExperiment<VType>;

  std::vector<benchmark::Approach> approaches = {
    { cas::IndexType::DynamicInterleaving, "dy",  false, 0, 0 },
    { cas::IndexType::ZOrder,              "zo",  true, 11, 3 },
    { cas::IndexType::LabelWise,           "lw",  false, 0, 0 },
    { cas::IndexType::PathValue,           "pv",  false, 0, 0 },
    { cas::IndexType::ValuePath,           "vp",  false, 0, 0 },
    { cas::IndexType::Xml,                 "xml", false, 0, 0 },
  };

  std::vector<cas::SearchKey<VType>> queries = {
    Query<VType>("/Cell_Phones_&_Accessories^", 10000, 20000),
    Query<VType>("/Clothing,_Shoes_&_Jewelry/Women/?/Sweaters^",  7000, 10000),
  };

  Exp bm(dataset_folder + "/amazon/dataset.csv", ';',
      approaches, queries);
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
