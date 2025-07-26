#ifndef APP_SNIPPERAPP_H
#define APP_SNIPPERAPP_H

#include <app/ConfigManager.h>
#include <core/SnippetStore.h>
#include <core/SearchEngine.h>
#include <platform/ClipboardManager.h>
#include <memory>
#include <vector>
#include <string>
#include <optional>

namespace app {

class SnipperApp {
public:
  SnipperApp(int argc, char** argv);
  ~SnipperApp() = default;

  int run();

private:
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

  std::string getCurrentTimestamp();

  ConfigManager _config;
  core::SnippetStore _store;
  std::unique_ptr<core::SearchEngine> _search;

  std::vector<std::string> _args;
};

}  // namespace app

#endif  // APP_SNIPPERAPP_H
