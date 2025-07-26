#ifndef PLATFORM_HOTKEYLISTENER_H
#define PLATFORM_HOTKEYLISTENER_H

#include <functional>
#include <string>
#include <map>

namespace platform {

class HotkeyListener {
public:
  using Callback = std::function<void()>;

  HotkeyListener();
  ~HotkeyListener();

  // Register a named hotkey string → callback
  void registerHotkey(const std::string& hotkey, Callback cb);

  // Start/stop the listening loop
  void start();
  void stop();

private:
  // store all key→callback mappings
  std::map<std::string, Callback> _callbacks;
  bool                             _running = false;
};

}  // namespace platform

#endif  // PLATFORM_HOTKEYLISTENER_H
