#ifndef CAS_SURROGATE_H_
#define CAS_SURROGATE_H_

#include <unordered_map>
#include <string>
#include <vector>


namespace cas {

class Surrogate {
  uint32_t counter_ = 0;
  std::unordered_map<std::string, uint32_t> map_;
  std::unordered_map<uint32_t, std::string> map_inv_;

public:
  const size_t max_depth_;
  const size_t bytes_per_label_;

  Surrogate(size_t max_depth, size_t bytes_per_label);

  std::vector<uint8_t> MapLabel(const std::string& label);

  std::vector<uint8_t> MapPath(const std::vector<std::string>& path);

  std::string MapLabelInv(const std::vector<uint8_t>& bytes);

  size_t NrBytes() const;

};

} // namespace cas


#endif // CAS_SURROGATE_H_
