#ifndef CAS_TYPES_H_
#define CAS_TYPES_H_


#include <array>
#include <cstdint>
#include <vector>
#include <string>


namespace cas {

/* using ref_t = uint64_t; */
using ref_t = std::array<uint8_t, 20>;
const ref_t DEFAULT_REF{
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00
};

std::string RefToString(const ref_t& ref);
ref_t RefFromString(const std::string& ref);

// value types
using vint32_t = int32_t;
using vint64_t = int64_t;
using vstring_t = std::string;
using vbool_t = bool;
using vfloat_t = double;

const vint32_t  VINT32_MIN  = INT32_MIN;
const vint32_t  VINT32_MAX  = INT32_MAX;
const vint64_t  VINT64_MIN  = INT64_MIN;
const vint64_t  VINT64_MAX  = INT64_MAX;
const vstring_t VSTRING_MIN = "";
const vstring_t VSTRING_MAX({ static_cast<char>(0xFF), static_cast<char>(0x00) });

} // namespace cas

#endif //  CASXML_TYPES_H_
