#include <app/ConfigManager.h>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <platform/PlatformUtils.h>

using json = nlohmann::json;

#ifndef DEFAULT_CONFIG_PATH
#error "DEFAULT_CONFIG_PATH not defined"
#endif

#ifndef DEFAULT_SNIPPETS_DB
#error "DEFAULT_SNIPPETS_DB not defined"
#endif

#ifndef DEFAULT_PROJECT_ROOT
#error "DEFAULT_PROJECT_ROOT not defined"
#endif

namespace app {

ConfigManager::ConfigManager(int argc, char** argv) {
  loadDefaults();

  // Only override config if user passed a .json file path
  if (argc > 1) {
    std::string arg1 = argv[1];
    if (arg1.size() >= 5 && arg1.substr(arg1.size() - 5) == ".json") {
      loadUserConfig(arg1);
    }
  }
}

void ConfigManager::loadDefaults() {
  std::string configPath = DEFAULT_CONFIG_PATH;
  std::ifstream in(configPath);
  if (!in.is_open())
    throw std::runtime_error("Cannot open default config: " + configPath);

  _configDir = platform::PlatformUtils::getDirname(configPath);

  json j;
  in >> j;
  merge(j);

  if (_snippetsDbPath.empty()) {
    _snippetsDbPath = DEFAULT_SNIPPETS_DB;
  }
}

void ConfigManager::loadUserConfig(const std::string& path) {
  std::ifstream in(path);
  if (!in.is_open())
    throw std::runtime_error("Cannot open user config: " + path);

  _configDir = platform::PlatformUtils::getDirname(path);

  json j;
  in >> j;
  merge(j);
}

void ConfigManager::merge(const json& j) {
  if (j.contains("snippetsDbPath")) {
    std::string dbPath = j["snippetsDbPath"].get<std::string>();

    if (!std::filesystem::path(dbPath).is_absolute()) {
      dbPath = (std::filesystem::path(DEFAULT_PROJECT_ROOT) / dbPath).string();
    }

    _snippetsDbPath = dbPath;
  }

  if (j.contains("hotkeys")) {
    for (auto& [k, v] : j["hotkeys"].items())
      _hotkeys[k] = v.get<std::string>();
  }

  if (j.contains("ui")) {
    auto u = j["ui"];
    if (u.contains("theme"))      _uiPrefs.theme      = u["theme"].get<std::string>();
    if (u.contains("fontFamily")) _uiPrefs.fontFamily = u["fontFamily"].get<std::string>();
    if (u.contains("fontSize"))   _uiPrefs.fontSize   = u["fontSize"].get<int>();
  }
}

const std::string& ConfigManager::snippetsDbPath() const {
  return _snippetsDbPath;
}

const std::map<std::string, std::string>& ConfigManager::hotkeys() const {
  return _hotkeys;
}

const UiPrefs& ConfigManager::uiPrefs() const {
  return _uiPrefs;
}

}  // namespace app
