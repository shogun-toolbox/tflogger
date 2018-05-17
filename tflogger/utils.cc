#include <tflogger/utils.h>

#ifdef _MSC_VER
#include <Windows.h>
#else
#include <unistd.h>
#endif

void tflogger::utils::EncodeFixed16(char* buf, uint16_t value)
{
  if (tflogger::utils::kLittleEndian) {
    memcpy(buf, &value, sizeof(value));
  } else {
    buf[0] = value & 0xff;
    buf[1] = (value >> 8) & 0xff;
  }
}

void tflogger::utils::EncodeFixed32(char* buf, uint32_t value)
{
  if (tflogger::utils::kLittleEndian) {
    memcpy(buf, &value, sizeof(value));
  } else {
    buf[0] = value & 0xff;
    buf[1] = (value >> 8) & 0xff;
    buf[2] = (value >> 16) & 0xff;
    buf[3] = (value >> 24) & 0xff;
  }
}

void tflogger::utils::EncodeFixed64(char* buf, uint64_t value)
{
  if (tflogger::utils::kLittleEndian) {
    memcpy(buf, &value, sizeof(value));
  } else {
    buf[0] = value & 0xff;
    buf[1] = (value >> 8) & 0xff;
    buf[2] = (value >> 16) & 0xff;
    buf[3] = (value >> 24) & 0xff;
    buf[4] = (value >> 32) & 0xff;
    buf[5] = (value >> 40) & 0xff;
    buf[6] = (value >> 48) & 0xff;
    buf[7] = (value >> 56) & 0xff;
  }
}

std::string tflogger::utils::Hostname()
{
#ifdef _MSC_VER
    char name[1024];
    DWORD name_size = sizeof(name);
    name[0] = 0;
    if (::GetComputerNameA(name, &name_size))
        name[name_size] = 0;
    return name;
#else
    char hostname[1024];
    gethostname(hostname, sizeof hostname);
    hostname[sizeof hostname - 1] = 0;
    return std::string(hostname);
#endif
}
