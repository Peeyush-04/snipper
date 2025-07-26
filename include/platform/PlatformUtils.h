#ifndef PLATFORM_PLATFORMUTILS_H
#define PLATFORM_PLATFORMUTILS_H

#include <string>

namespace platform {

class PlatformUtils {
public:
  static std::string getExecutableDir();
  static void sleepMs(int ms);
  static std::string getDirname(const std::string& path);
  static bool isAbsolutePath(const std::string& path);
};

}  // namespace platform

#endif  // PLATFORM_PLATFORMUTILS_H
