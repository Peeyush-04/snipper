#include <platform/PlatformUtils.h>
#include <filesystem>
#ifdef _WIN32
  #include <windows.h>
  #include <shlwapi.h>
#else
  #include <unistd.h>
  #include <limits.h>
#endif

namespace platform {

std::string PlatformUtils::getExecutableDir() {
#ifdef _WIN32
  char path[MAX_PATH];
  GetModuleFileNameA(NULL, path, MAX_PATH);
  PathRemoveFileSpecA(path);
  return std::string(path);
#else
  char path[PATH_MAX];
  ssize_t len = ::readlink("/proc/self/exe", path, sizeof(path)-1);
  if (len != -1) {
    path[len] = '\\0';
    std::string s(path);
    return s.substr(0, s.find_last_of('/'));
  }
  return ".";
#endif
}

void PlatformUtils::sleepMs(int ms) {
#ifdef _WIN32
  Sleep(ms);
#else
  usleep(ms * 1000);
#endif
}

std::string PlatformUtils::getDirname(const std::string& path) {
  return std::filesystem::path(path).parent_path().string();
}

bool PlatformUtils::isAbsolutePath(const std::string& path) {
  return std::filesystem::path(path).is_absolute();
}

}  // namespace platform
