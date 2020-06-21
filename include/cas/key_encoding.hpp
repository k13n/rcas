#ifndef CAS_KEY_ENCODING_H_
#define CAS_KEY_ENCODING_H_

namespace cas {

enum class PathMask : uint8_t {
  Label         = 0xFC,
  Wildcard      = 0xFD,
  Descendant    = 0xFE,
  PathSeperator = 0xFF,
};

// special byte sequences used for encoding/decoding
const uint8_t kNullByte = 0x00;
const uint8_t kPathSep  = 0xFF;

const uint64_t kMsbMask64 = (1UL << (64-1));
const uint32_t kMsbMask32 = (1UL << (32-1));

const uint8_t kNullTrue  = 0x01;
const uint8_t kNullFalse = 0x00;

// maximum length of encoded values
const size_t kMaxValueLength = 10000;
const size_t kMaxPathLength  = 10000;

} // namespace cas

#endif // CAS_KEY_ENCODING_H_
