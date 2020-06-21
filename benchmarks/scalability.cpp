#include "benchmark/benchmarks.hpp"


int main_(int argc, char** argv) {
  std::string dataset_folder = "/storage/";
  if (argc >= 2) {
    dataset_folder = argv[1];
  }

  using Exp = benchmark::ScalabilityExperiment;

  std::vector<benchmark::Approach> approaches = {
    { cas::IndexType::DynamicInterleaving, "dy",  false, 0, 0 },
    { cas::IndexType::ZOrder,              "zo",  true, 21, 3 },
    { cas::IndexType::LabelWise,           "lw",  false, 0, 0 },
    { cas::IndexType::PathValue,           "pv",  false, 0, 0 },
    { cas::IndexType::ValuePath,           "vp",  false, 0, 0 },
    { cas::IndexType::Xml,                 "xml", false, 0, 0 },
  };

  std::vector<Exp::Dataset> datasets = {
    { 100000000, dataset_folder + "/sf_dataset/scalability_size/n100000000.csv" },
    {  10000000, dataset_folder + "/sf_dataset/scalability_size/n10000000.csv"  },
    {   1000000, dataset_folder + "/sf_dataset/scalability_size/n1000000.csv"   },
    {    100000, dataset_folder + "/sf_dataset/scalability_size/n100000.csv"    },
    {     10000, dataset_folder + "/sf_dataset/scalability_size/n10000.csv"     },
    {      1000, dataset_folder + "/sf_dataset/scalability_size/n1000.csv"      },
    {       100, dataset_folder + "/sf_dataset/scalability_size/n100.csv"       },
    {        10, dataset_folder + "/sf_dataset/scalability_size/n10.csv"        },
    {         1, dataset_folder + "/sf_dataset/scalability_size/n1.csv"         },
  };

  Exp bm(datasets, ';', approaches);
  bm.Run();

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
