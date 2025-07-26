#include <platform/HotkeyListener.h>
#include <thread>
#include <chrono>

namespace platform {

HotkeyListener::HotkeyListener() = default;
HotkeyListener::~HotkeyListener() { stop(); }

void HotkeyListener::registerHotkey(
    const std::string& hotkey, Callback cb) {
  _callbacks[hotkey] = cb;
}

void HotkeyListener::start() {
  _running = true;
  // In a real impl you'd hook into OS APIs. Here we simply spin.
  std::thread([this](){
    while (_running) {
      // Polling stub: in practice listen for key events
      std::this_thread::sleep_for(std::chrono::seconds(1));
      // e.g. if (event == someHotkey) _callbacks["..."]();
    }
  }).detach();
}

void HotkeyListener::stop() {
  _running = false;
}

}  // namespace platform
