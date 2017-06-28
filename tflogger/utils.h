#ifndef __TFLOGGER_UTILS_H__
#define __TFLOGGER_UTILS_H__

#include <tflogger/crc32/crc32c.h>

#include <cstring>
#include <string>

#ifdef __STDC_FORMAT_MACROS
#include <inttypes.h>
#else
#define __STDC_FORMAT_MACROS 1
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS
#endif

namespace tflogger
{
namespace utils
{
    constexpr bool kLittleEndian = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__;
    
    std::string Hostname();

    void EncodeFixed16(char* dst, uint16_t value);
    void EncodeFixed32(char* dst, uint32_t value);
    void EncodeFixed64(char* dst, uint64_t value);

    inline uint32_t DecodeFixed32(const char* ptr)
    {
        if (kLittleEndian)
        {
            // Load the raw bytes
            uint32_t result;
            memcpy(&result, ptr, sizeof(result));  // gcc optimizes this to a plain load
            return result;
        } else {
            return ((static_cast<uint32_t>(static_cast<unsigned char>(ptr[0]))) |
                    (static_cast<uint32_t>(static_cast<unsigned char>(ptr[1])) << 8) |
                    (static_cast<uint32_t>(static_cast<unsigned char>(ptr[2])) << 16) |
                    (static_cast<uint32_t>(static_cast<unsigned char>(ptr[3])) << 24));
        }
    }

    inline uint64_t DecodeFixed64(const char* ptr)
    {
      if (kLittleEndian)
      {
        // Load the raw bytes
        uint64_t result;
        memcpy(&result, ptr, sizeof(result));  // gcc optimizes this to a plain load
        return result;
      } 
      else 
      {
        uint64_t lo = DecodeFixed32(ptr);
        uint64_t hi = DecodeFixed32(ptr + 4);
        return (hi << 32) | lo;
      }
    }

    inline uint32_t MaskedCrc(const char* data, size_t n)
    {
        return crc32c::Mask(crc32c::Value(data, n));
    }
}
}

#endif /* __TFLOGGER_UTILS_H__ */

