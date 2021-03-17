#include "benchmark/benchmarks.hpp"


int main_(int argc, char** argv) {
  using Exp = benchmark::ScalabilityExperiment2;

  if (argc < 2) {
    return 1;
  }

  size_t dataset_size;
  std::sscanf(argv[1], "%zu", &dataset_size);

  size_t page_size = 16384;
  std::string filename = "/local/scratch/wellenzohn/datasets/dataset.16384.partition";
  std::vector<Exp::Dataset> datasets = {
    { filename, page_size,   dataset_size },
  };

  /* std::vector<Exp::Dataset> datasets = { */
  /*   { filename, page_size,   1'000'000'000 }, */
  /*   /1* { filename, page_size,  10'000'000'000 }, *1/ */
  /*   /1* { filename, page_size,  50'000'000'000 }, *1/ */
  /*   /1* { filename, page_size, 100'000'000'000 }, *1/ */
  /*   /1* { filename, page_size, 150'000'000'000 }, *1/ */
  /*   /1* { filename, page_size, 200'000'000'000 }, *1/ */
  /*   /1* { filename, page_size, 250'000'000'000 }, *1/ */
  /*   /1* { filename, page_size, 300'000'000'000 }, *1/ */
  /* }; */

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
