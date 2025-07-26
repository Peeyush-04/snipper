#ifndef CORE_SNIPPETSTORE_H
#define CORE_SNIPPETSTORE_H

#include <string>
#include <vector>
#include <models/Snippet.h>

namespace core {

class SnippetStore {
public:
  explicit SnippetStore(const std::string& dbPath);

  // Load all snippets (throws std::runtime_error on error)
  std::vector<model::Snippet> loadSnippets();

  // Save back to disk
  void saveSnippets(const std::vector<model::Snippet>& snippets);

  // Convenience for CRUD:
  void addSnippet(const model::Snippet& s);
  void updateSnippet(const model::Snippet& s);
  void removeSnippet(const std::string& id);

private:
  std::string _dbPath;
};

}  // namespace core

#endif  // CORE_SNIPPETSTORE_H
