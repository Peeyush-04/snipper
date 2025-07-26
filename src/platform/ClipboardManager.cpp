#include <platform/ClipboardManager.h>
#ifdef _WIN32
  #include <windows.h>
#endif

namespace platform {

void ClipboardManager::writeText(const std::string& text) {
#ifdef _WIN32
  if (!OpenClipboard(nullptr)) return;
  EmptyClipboard();
  HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, text.size()+1);
  memcpy(GlobalLock(h), text.c_str(), text.size()+1);
  GlobalUnlock(h);
  SetClipboardData(CF_TEXT, h);
  CloseClipboard();
#else
  // no‑op or add X11/macOS impl
#endif
}

std::string ClipboardManager::readText() {
#ifdef _WIN32
  if (!OpenClipboard(nullptr)) return "";
  HGLOBAL h = GetClipboardData(CF_TEXT);
  if (!h) { CloseClipboard(); return ""; }
  char* data = static_cast<char*>(GlobalLock(h));
  std::string s(data);
  GlobalUnlock(h);
  CloseClipboard();
  return s;
#else
  return "";
#endif
}

}  // namespace platform
