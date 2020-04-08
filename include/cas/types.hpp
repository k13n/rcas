#ifndef CAS_TYPES_H_
#define CAS_TYPES_H_


#include <cstdint>
#include <vector>
#include <string>


namespace cas {

using ref_t = uint64_t;
using path_t = std::vector<std::string>;

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
