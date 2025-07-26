#include <core/SearchEngine.h>
#include <algorithm>

namespace core {

SearchEngine::SearchEngine(const std::vector<model::Snippet>& snippets)
  : _snips(snippets) {}

std::vector<model::Snippet> SearchEngine::fuzzySearch(
    const std::string& query, size_t maxResults) {
  std::vector<std::pair<int,model::Snippet>> scored;
  for (auto& s : _snips) {
    int sc = score(s, query);
    if (sc > 0)
      scored.emplace_back(sc, s);
  }
  std::sort(scored.begin(), scored.end(),
            [](auto& a, auto& b){ return a.first > b.first; });

  std::vector<model::Snippet> out;
  for (size_t i = 0; i < scored.size() && i < maxResults; ++i)
    out.push_back(scored[i].second);
  return out;
}

int SearchEngine::score(const model::Snippet& s, const std::string& q) {
  if (q.empty()) return 1;
  int score = 0;
  if (s.title.find(q) != std::string::npos) score += 10;
  if (s.content.find(q) != std::string::npos) score += 5;
  for (auto& tag : s.tags)
    if (tag == q) score += 8;
  return score;
}

}  // namespace core
