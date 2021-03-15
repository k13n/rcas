#include "benchmark/benchmarks.hpp"


int main_(int argc, char** argv) {
  std::string dataset_folder = "/storage/";
  if (argc >= 2) {
    dataset_folder = argv[1];
  }

  using Exp = benchmark::ScalabilityExperiment2;

  size_t page_size = 16384;
  std::string filename = "/home/user/wellenzohn/datasets/gitlab_all/dataset.200GB.16384.partition";
  std::vector<Exp::Dataset> datasets = {
    { filename, page_size,   1'000'000'000 },
    { filename, page_size,  10'000'000'000 },
    { filename, page_size,  50'000'000'000 },
    { filename, page_size, 100'000'000'000 },
  };

  Exp bm(datasets);
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
