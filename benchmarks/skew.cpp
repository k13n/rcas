#include "benchmark/benchmarks.hpp"


int main(int argc, char** argv) {
  std::string dataset_folder = "/storage/";
  if (argc >= 2) {
    dataset_folder = argv[1];
  }

  using VType = cas::vint64_t;
  using Exp = benchmark::SkewExperiment<VType>;
  std::string query_path = "/usr/lib^";

  VType min = 1575385560;

  std::string path = dataset_folder + "/sf_dataset/skew/sf_ctime_";
  std::vector<Exp::Dataset> datasets = {
    { 10000000, 0.00, path+"10000000_000.csv", min, 1815385500 },
    { 10000000, 0.25, path+"10000000_025.csv", min, 1752220260 },
    { 10000000, 0.50, path+"10000000_050.csv", min, 1671452040 },
    { 10000000, 0.75, path+"10000000_075.csv", min, 1592204700 },
    { 10000000, 1.00, path+"10000000_100.csv", min, 1575412260 },
    { 10000000, 1.25, path+"10000000_125.csv", min, 1575385740 },
    { 10000000, 1.50, path+"10000000_150.csv", min, 1575385560 },
    {    10000, 0.00, path+"10000_000.csv"   , min, 1575625500 },
    {    10000, 0.25, path+"10000_025.csv"   , min, 1575562560 },
    {    10000, 0.50, path+"10000_050.csv"   , min, 1575483600 },
    {    10000, 0.75, path+"10000_075.csv"   , min, 1575410460 },
    {    10000, 1.00, path+"10000_100.csv"   , min, 1575387180 },
    {    10000, 1.25, path+"10000_125.csv"   , min, 1575385680 },
    {    10000, 1.50, path+"10000_150.csv"   , min, 1575385560 },
  };

  std::vector<benchmark::Approach> approaches = {
    { cas::IndexType::DynamicInterleaving, "dy",  false, 0, 0 },
    { cas::IndexType::ZOrder,              "zo",  true, 21, 3 },
    { cas::IndexType::LabelWise,           "lw",  false, 0, 0 },
    { cas::IndexType::PathValue,           "pv",  false, 0, 0 },
    { cas::IndexType::ValuePath,           "vp",  false, 0, 0 },
    { cas::IndexType::Xml,                 "xml", false, 0, 0 },
  };

  Exp bm(query_path, approaches, datasets, ';');
  bm.Run();

  return 0;
}
