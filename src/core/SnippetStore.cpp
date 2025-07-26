#include <core/SnippetStore.h>
#include <fstream>
#include <stdexcept>
#include <algorithm>

using json = nlohmann::json;

namespace core {

  SnippetStore::SnippetStore(const std::string& dbPath)
    : _dbPath(dbPath) {}

  std::vector<model::Snippet> SnippetStore::loadSnippets() {
    std::ifstream in(_dbPath);
    if (!in.is_open()) {
      // Try to create an empty DB
      std::ofstream out(_dbPath);
      if (!out.is_open())
        throw std::runtime_error("Cannot create missing snippets DB: " + _dbPath);
      out << "[]\n";  // Write empty JSON array
      out.close();

      // Now reopen it for reading
      in.open(_dbPath);
      if (!in.is_open())
        throw std::runtime_error("Cannot open newly created snippets DB: " + _dbPath);
    }

    json j; in >> j;

    std::vector<model::Snippet> outSnippets;
    for (auto& item : j) {
      outSnippets.push_back(model::Snippet::fromJson(item));
    }
    return outSnippets;
  }

  void SnippetStore::saveSnippets(const std::vector<model::Snippet>& snippets) {
    json j = json::array();
    for (auto& s : snippets)
      j.push_back(s.toJson());

    std::ofstream out(_dbPath);
    if (!out.is_open())
      throw std::runtime_error("Cannot write snippets DB: " + _dbPath);
    out << j.dump(2) << "\n";
  }

  void SnippetStore::addSnippet(const model::Snippet& s) {
    auto snippets = loadSnippets();

    // Prevent adding a snippet with duplicate ID
    auto it = std::find_if(snippets.begin(), snippets.end(), [&](const model::Snippet& existing) {
      return existing.id == s.id;
    });

    if (it != snippets.end()) {
      throw std::runtime_error("Snippet with ID already exists: " + s.id);
    }

    snippets.push_back(s);
    saveSnippets(snippets);
  }

  void SnippetStore::updateSnippet(const model::Snippet& s) {
    auto list = loadSnippets();
    bool found = false;
    for (auto& orig : list) {
      if (orig.id == s.id) {
        orig = s;
        found = true;
        break;
      }
    }
    if (!found)
      throw std::runtime_error("Snippet not found: " + s.id);
    saveSnippets(list);
  }

  void SnippetStore::removeSnippet(const std::string& id) {
    auto list = loadSnippets();
    auto it = std::remove_if(list.begin(), list.end(),
      [&](auto& sn){ return sn.id == id; });
    if (it == list.end())
      throw std::runtime_error("Snippet not found: " + id);
    list.erase(it, list.end());
    saveSnippets(list);
  }

  std::vector<model::Snippet> SnippetStore::getAllSnippets() {
    return loadSnippets();
  }

  std::optional<model::Snippet> SnippetStore::getSnippetById(const std::string& id) {
    auto list = loadSnippets();
    for (const auto& sn : list) {
      if (sn.id == id) return sn;
    }
    return std::nullopt;
  }

  std::vector<model::Snippet> SnippetStore::getSnippetsByTags(const std::vector<std::string>& tags) {
    auto list = loadSnippets();
    std::vector<model::Snippet> result;

    for (const auto& sn : list) {
      for (const auto& tag : tags) {
        if (std::find(sn.tags.begin(), sn.tags.end(), tag) != sn.tags.end()) {
          result.push_back(sn);
          break;
        }
      }
    }
    return result;
  }

  void SnippetStore::renameSnippet(const std::string& oldId, const std::string& newId) {
    auto list = loadSnippets();
    bool found = false;

    for (auto& sn : list) {
      if (sn.id == oldId) {
        sn.id = newId;
        found = true;
        break;
      }
    }

    if (!found)
      throw std::runtime_error("Snippet not found for rename: " + oldId);

    saveSnippets(list);
  }

  void SnippetStore::clearAll() {
    saveSnippets({});
  }

}  // namespace core
