#ifndef CORE_SNIPPETSTORE_H
#define CORE_SNIPPETSTORE_H

#include <models/Snippet.h>
#include <string>
#include <vector>
#include <optional>

namespace core {

class SnippetStore {
public:
  explicit SnippetStore(const std::string& dbPath);

  std::vector<model::Snippet> loadSnippets();
  void saveSnippets(const std::vector<model::Snippet>& snippets);

  void addSnippet(const model::Snippet& s);
  void updateSnippet(const model::Snippet& s);
  void removeSnippet(const std::string& id);

  std::vector<model::Snippet> getAllSnippets();
  std::optional<model::Snippet> getSnippetById(const std::string& id);
  std::vector<model::Snippet> getSnippetsByTags(const std::vector<std::string>& tags);
  void renameSnippet(const std::string& oldId, const std::string& newId);
  void clearAll();

private:
  std::string _dbPath;
};

}  // namespace core

#endif  // CORE_SNIPPETSTORE_H
