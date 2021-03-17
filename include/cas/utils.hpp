#ifndef CAS_UTILS_H_
#define CAS_UTILS_H_

#include <ostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstddef>


namespace cas {

class Utils {
public:
  static void DumpHexValues(const std::vector<uint8_t>& buffer);

  static void DumpHexValues(const std::vector<uint8_t>& buffer, size_t size);

  static void DumpHexValues(const std::vector<uint8_t>& buffer, size_t offset, size_t size);

  static void DumpChars(std::vector<uint8_t>& buffer);

  static void DumpChars(std::vector<uint8_t>& buffer, size_t size);

  static void DumpChars(std::vector<uint8_t>& buffer, size_t offset, size_t size);

  static void DumpChars(std::vector<uint8_t>& buffer, size_t offset,
                        size_t size, std::ostream& os);

  /*
   * returns:
   * -1 if lhs < rhs
   *  0 if lhs = rhs
   * +1 if lhs > rhs
   */
  static int Memcmp(const void* lhs, size_t len_lhs, const void* rhs, size_t len_rhs);

  template<class VType>
  static std::string TypeToString();

  static std::string CurrentIsoTime();
  static void Log(const std::string& msg);
};

} // namespace cas

#endif // CAS_UTILS_H_
