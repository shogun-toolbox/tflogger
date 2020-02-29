#include "tflogger/utils.h"

#ifdef _MSC_VER
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <sstream>

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

const std::string tflogger::utils::supportedFileVersion() {
    static const std::string kFileVersion = []() -> std::string {
        std::ostringstream fileVersion(tflogger::utils::kVersionPrefix, std::ios_base::ate);
        fileVersion << tflogger::utils::kCurrentVersion;
        return fileVersion.str();
    }();
    return kFileVersion;
}
