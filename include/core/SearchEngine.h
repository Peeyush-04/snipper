#ifndef CORE_SEARCHENGINE_H
#define CORE_SEARCHENGINE_H

#include <vector>
#include <string>
#include <models/Snippet.h>

namespace core {

class SearchEngine {
public:
  explicit SearchEngine(const std::vector<model::Snippet>& snippets);

  std::vector<model::Snippet> fuzzySearch(
    const std::string& query,
    size_t maxResults = 5);

private:
  std::vector<model::Snippet> _snips;
  int score(const model::Snippet& s, const std::string& q);
};

}  // namespace core

#endif  // CORE_SEARCHENGINE_H
