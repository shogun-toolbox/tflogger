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
    // Prefix of version string present in the first entry of every event file.
    static constexpr const char* kVersionPrefix = "brain.Event:";
    static constexpr const int kCurrentVersion = 2;
    const std::string supportedFileVersion();

    constexpr bool kLittleEndian = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__;

    std::string Hostname();

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

    inline uint32_t MaskedCrc(const char* data, size_t n)
    {
        return crc32c::Mask(crc32c::Value(data, n));
    }
}
}

#endif /* __TFLOGGER_UTILS_H__ */

