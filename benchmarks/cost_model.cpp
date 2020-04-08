#include "benchmark/benchmarks.hpp"


void DatasetSF(std::string dataset_folder) {
  using VType = cas::vint64_t;
  using Exp = benchmark::CostExperiment<VType>;

  std::vector<benchmark::Approach> approaches = {
    { cas::IndexType::DynamicInterleaving, "dy",  false,  0, 0 },
  };

  std::vector<cas::SearchKey<VType>> queries = {
    Query<VType>("/usr/include^", 5000, cas::VINT64_MAX),
    Query<VType>("/usr/include^", 3000, 4000),
    Query<VType>("/usr/lib^", 0, 1000),
    Query<VType>("/usr/share^Makefile", 1000, 2000),
    Query<VType>("/usr/share/doc^README", 4000, 5000),
    Query<VType>("/etc^", 5000, cas::VINT64_MAX),
    //
    Query<VType>("/usr/share^", 1000, 2000),
    Query<VType>("/usr/share/doc^", 4000, 5000),
  };

  Exp bm(dataset_folder + "/sf_dataset/original/sf_size_unique.csv", ';',
      approaches, queries);
  bm.Run();
}


void DatasetXMark(std::string dataset_folder) {
  using VType = cas::vint32_t;
  using Exp = benchmark::CostExperiment<VType>;

  std::vector<benchmark::Approach> approaches = {
    { cas::IndexType::DynamicInterleaving, "dy",  false,  0, 0 },
  };

  std::vector<cas::SearchKey<VType>> queries = {
    Query<VType>("/site/people^interest", 0,  50000),
    Query<VType>("/site/regions/africa^", 0,  50000),
  };

  Exp bm(dataset_folder + "/xmark/category_unique.csv", ';',
      approaches, queries);
  bm.Run();
}


void DatasetAmazon(std::string dataset_folder) {
  using VType = cas::vint32_t;
  using Exp = benchmark::CostExperiment<VType>;

  std::vector<benchmark::Approach> approaches = {
    { cas::IndexType::DynamicInterleaving, "dy",  false,  0, 0 },
  };

  std::vector<cas::SearchKey<VType>> queries = {
    Query<VType>("/Cell_Phones_&_Accessories^", 10000, 20000),
    Query<VType>("/Clothing,_Shoes_&_Jewelry/Women/?/Sweaters^",  7000, 10000),
    //
    Query<VType>("/Clothing,_Shoes_&_Jewelry/Women^",  7000, 10000),
  };

  Exp bm(dataset_folder + "/amazon/dataset_unique.csv", ';',
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
