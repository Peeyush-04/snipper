#ifndef APP_SNIPPERAPP_H
#define APP_SNIPPERAPP_H

#include <app/ConfigManager.h>
#include <core/SnippetStore.h>
#include <core/SearchEngine.h>
#include <platform/ClipboardManager.h>

#include <string>
#include <memory>
#include <vector>
#include <optional>

namespace app {

class SnipperApp {
public:
  SnipperApp(int argc, char** argv);
  ~SnipperApp() = default;

  int run();

  // Utility to normalize snippet IDs (e.g., "0001" -> "1")
  std::string normalizeId(const std::string& id) {
    size_t pos = id.find_first_not_of('0');
    return (pos == std::string::npos) ? "0" : id.substr(pos);
  }

private:
  // CLI command handlers
  void printHelp();
  void listSnippets();
  void listSnippetsByTags(const std::string& tagCsv);
  void addSnippet();
  void removeSnippet(const std::string& id);
  void copySnippet(const std::string& id);
  void showSnippet(const std::string& id);
  void searchSnippets(const std::string& query);
  void editSnippet(const std::string& id);
  void importSnippets(const std::string& filePath, bool overwrite = false);
  void exportSnippets(const std::optional<std::string>& idOpt, const std::string& filePath);
  void initStore(const std::optional<std::string>& nameOpt);
  void renameSnippet(const std::string& oldId, const std::string& newId);
  void sortSnippets();
  void clearSnippets();
  void showStats();

  // Utility
  std::string getCurrentTimestamp();

  // Core components
  ConfigManager _config;
  core::SnippetStore _store;
  std::unique_ptr<core::SearchEngine> _search;
  std::vector<std::string> _args;
};

}  // namespace app

#endif  // APP_SNIPPERAPP_H
