#include "benchmark/benchmarks.hpp"


int main_(int argc, char** argv) {
  using Exp = benchmark::ScalabilityExperiment2;

  if (argc < 3) {
    return 1;
  }

  size_t dataset_size;
  std::sscanf(argv[1], "%zu", &dataset_size);

  size_t page_size = 16384;
  std::string filename{argv[2]};
  std::vector<Exp::Dataset> datasets = {
    { filename, page_size,   dataset_size },
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
