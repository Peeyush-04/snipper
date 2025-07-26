#ifndef PLATFORM_CLIPBOARDMANAGER_H
#define PLATFORM_CLIPBOARDMANAGER_H

#include <string>

namespace platform {

class ClipboardManager {
public:
  static void writeText(const std::string& text);
  static std::string readText();
};

}  // namespace platform

#endif  // PLATFORM_CLIPBOARDMANAGER_H
