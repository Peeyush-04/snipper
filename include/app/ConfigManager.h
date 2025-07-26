#ifndef APP_CONFIGMANAGER_H
#define APP_CONFIGMANAGER_H

#include <string>
#include <map>
#include <external/json.hpp>

namespace app {

struct UiPrefs {
  std::string theme;      // "light" or "dark"
  std::string fontFamily;
  int         fontSize;
};

class ConfigManager {
public:
  ConfigManager(int argc, char** argv);

  // from default_config.json / override config.json
  const std::string& snippetsDbPath() const;
  const std::map<std::string,std::string>& hotkeys() const;
  const UiPrefs& uiPrefs() const;

private:
  void loadDefaults();
  void loadUserConfig(const std::string& path);
  void merge(const nlohmann::json& j);

  std::string                        _snippetsDbPath;
  std::map<std::string,std::string>  _hotkeys;
  UiPrefs                            _uiPrefs;
  std::string _configDir;
};

}  // namespace app

#endif  // APP_CONFIGMANAGER_H
