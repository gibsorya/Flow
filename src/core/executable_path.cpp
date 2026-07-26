#include "core/executable_path.h"

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
    #include <vector>
#elif defined(__linux__)
    #include <unistd.h>
#endif

std::filesystem::path executableDir() {
    namespace fs = std::filesystem;

#if defined(_WIN32)
    char buf[MAX_PATH];
    DWORD len = GetModuleFileNameA(nullptr, buf, MAX_PATH);
    fs::path exePath(std::string(buf, len));

#elif defined(__APPLE__)
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);      // first call: get length
    std::vector<char> buf(size);
    _NSGetExecutablePath(buf.data(), &size);   // second call: fill it
    fs::path exePath = fs::canonical(buf.data());  // resolve any symlinks

#elif defined(__linux__)
    fs::path exePath = fs::canonical("/proc/self/exe");

#else
    #error "executableDir not implemented for this platform"
#endif

    return exePath.parent_path();   // strip the filename, return the dir
}