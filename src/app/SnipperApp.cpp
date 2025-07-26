#include <iostream>
#include <app/SnipperApp.h>
#include <platform/ClipboardManager.h>
#include <models/Snippet.h>
#include <external/json.hpp>
#include <memory>
#include <set>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#ifndef DEFAULT_PROJECT_ROOT
#error "DEFAULT_PROJECT_ROOT not defined"
#endif

namespace app {

SnipperApp::SnipperApp(int argc, char** argv)
  : _config(argc, argv),
    _store(_config.snippetsDbPath()) {
  for (int i = 1; i < argc; ++i)
    _args.emplace_back(argv[i]);
}

int SnipperApp::run() {
  if (_args.empty()) {
    printHelp();
    return 0;
  }

  const std::string& command = _args[0];
  if (command == "init") {
    std::optional<std::string> nameOpt;
    if (_args.size() >= 2) nameOpt = _args[1];
    initStore(nameOpt);

  } else if (command == "list") {
    if (_args.size() >= 3 && _args[1] == "--tags") {
      listSnippetsByTags(_args[2]);
    } else {
      listSnippets();
    }

  } else if (command == "list-by-tags" && _args.size() >= 2) {
    listSnippetsByTags(_args[1]);

  } else if (command == "add") {
    addSnippet();

  } else if (command == "remove" && _args.size() >= 2) {
    removeSnippet(_args[1]);

  } else if (command == "copy" && _args.size() >= 2) {
    copySnippet(_args[1]);

  } else if (command == "show" && _args.size() >= 2) {
    showSnippet(_args[1]);

  } else if (command == "search" && _args.size() >= 2) {
    searchSnippets(_args[1]);

  } else if (command == "edit" && _args.size() >= 2) {
    editSnippet(_args[1]);

  } else if (command == "rename" && _args.size() >= 3) {
    renameSnippet(_args[1], _args[2]);

  } else if (command == "export") {
    std::optional<std::string> idOpt;
    std::string filePath = "snippets_export.json";
    for (size_t i = 1; i < _args.size(); ++i) {
      if (_args[i] == "--file" && i + 1 < _args.size()) {
        filePath = _args[i + 1];
        ++i;
      } else {
        idOpt = _args[i];
      }
    }
    exportSnippets(idOpt, filePath);

  } else if (command == "import") {
    if (_args.size() < 3) {
      std::cout << "Usage: snipper import <file_path> [--overwrite]\n";
      return 1;
    }
    std::string filePath = _args[2];
    bool overwrite = (_args.size() >= 4 && _args[3] == "--overwrite");
    importSnippets(filePath, overwrite);

  } else if (command == "sort") {
    sortSnippets();

  } else if (command == "clear") {
    clearSnippets();

  } else if (command == "stats") {
    showStats();

  } else if (command == "help") {
    printHelp();

  } else {
    std::cout << "Unknown command: " << command << "\n";
    printHelp();
  }

  return 0;
}

void SnipperApp::printHelp() {
  std::cout << "Snipper - Simple Snippet Manager\n\n";
  std::cout << "Usage:\n";
  std::cout << "  snipper <command> [options]\n\n";
  std::cout << "Commands:\n";
  std::cout << "  init [name]                 Initialize a new snippet store (optionally named)\n";
  std::cout << "  add                         Add a new snippet\n";
  std::cout << "  edit <id>                   Edit an existing snippet\n";
  std::cout << "  remove <id>                 Remove a snippet by ID\n";
  std::cout << "  copy <id>                   Copy snippet content to clipboard\n";
  std::cout << "  show <id>                   Display snippet content\n";
  std::cout << "  list                        List all snippets\n";
  std::cout << "  list-by-tags <tags>         List snippets filtered by comma-separated tags\n";
  std::cout << "  search <query>              Search snippets by content, title, or tags\n";
  std::cout << "  rename <old-id> <new-id>    Rename a snippet ID\n";
  std::cout << "  export [id] --file <file>   Export all or a specific snippet to a JSON file\n";
  std::cout << "  import <file> [--overwrite] Import snippets from JSON file (optional overwrite)\n";
  std::cout << "  sort                        Sort and list snippets by title\n";
  std::cout << "  clear                       Clear all snippets (with confirmation)\n";
  std::cout << "  stats                       Show statistics about your snippets\n";
  std::cout << "  help                        Show this help message\n\n";
}

void SnipperApp::listSnippets() {
  auto all = _store.loadSnippets();
  std::cout << "Snippets:\n";
  for (const auto& s : all) {
    std::cout << "- " << s.id << " | " << s.title << " | tags: ";
    for (const auto& t : s.tags) std::cout << t << " ";
    std::cout << "\n";
  }
}

void SnipperApp::addSnippet() {
  model::Snippet s;

  std::cout << "Enter ID: ";
  std::getline(std::cin, s.id);

  std::cout << "Enter Title: ";
  std::getline(std::cin, s.title);

  std::cout << "Enter Content (paste multiple lines and press Ctrl+D (Linux/macOS) or Ctrl+Z then Enter (Windows) to finish):\n";

  std::ostringstream contentStream;
  std::string line;
  while (std::getline(std::cin, line)) {
    contentStream << line << '\n';
  }
  s.content = contentStream.str();

  // Clear the EOF state so we can continue taking input
  std::cin.clear();

  std::cout << "Enter Tags (comma-separated): ";
  std::string tags;
  std::getline(std::cin, tags);

  std::stringstream ss(tags);
  std::string tag;
  while (std::getline(ss, tag, ',')) {
    if (!tag.empty()) s.tags.push_back(tag);
  }

  s.createdAt = s.updatedAt = getCurrentTimestamp();
  _store.addSnippet(s);
  std::cout << "Snippet added.\n";
}

void SnipperApp::removeSnippet(const std::string& id) {
  _store.removeSnippet(id);
  std::cout << "Snippet removed.\n";
}

void SnipperApp::copySnippet(const std::string& id) {
  auto all = _store.loadSnippets();
  std::string inputNorm = normalizeId(id);

  // Exact ID match
  auto it = std::find_if(all.begin(), all.end(), [&](const model::Snippet& s) {
    return normalizeId(s.id) == inputNorm;
  });

  if (it != all.end()) {
    platform::ClipboardManager::writeText(it->content);
    std::cout << "Copied to clipboard (by ID).\n";
    return;
  }

  // Partial title match
  auto itTitle = std::find_if(all.begin(), all.end(), [&](const model::Snippet& s) {
    return s.title.find(id) != std::string::npos;
  });

  if (itTitle != all.end()) {
    platform::ClipboardManager::writeText(itTitle->content);
    std::cout << "Copied to clipboard (by title).\n";
    return;
  }

  std::cout << "Snippet not found. Available:\n";
  for (const auto& s : all)
    std::cout << "  - " << s.id << " | " << s.title << "\n";
}

std::string SnipperApp::getCurrentTimestamp() {
  auto now = std::chrono::system_clock::now();
  std::time_t time = std::chrono::system_clock::to_time_t(now);
  std::ostringstream oss;
  oss << std::put_time(std::localtime(&time), "%FT%T");
  return oss.str();
}

void SnipperApp::showSnippet(const std::string& id) {
  auto all = _store.loadSnippets();
  std::string inputNorm = normalizeId(id);

  auto it = std::find_if(all.begin(), all.end(), [&](const model::Snippet& s) {
    return normalizeId(s.id) == inputNorm;
  });

  if (it == all.end()) {
    std::cout << "Snippet not found.\n";
    return;
  }

  const auto& s = *it;
  std::cout << "ID: " << s.id << "\n"
            << "Title: " << s.title << "\n"
            << "Tags: ";
  for (const auto& tag : s.tags) std::cout << tag << " ";
  std::cout << "\n"
            << "Created: " << s.createdAt << "\n"
            << "Updated: " << s.updatedAt << "\n\n"
            << "Content:\n" << s.content << "\n";
}

void SnipperApp::searchSnippets(const std::string& query) {
  auto all = _store.loadSnippets();
  _search = std::make_unique<core::SearchEngine>(all);
  auto results = _search->fuzzySearch(query, 10);  // top 10 matches

  if (results.empty()) {
    std::cout << "No snippets found.\n";
    return;
  }

  std::cout << "Search results:\n";
  for (const auto& s : results) {
    std::cout << "- " << s.id << " | " << s.title << " | tags: ";
    for (const auto& t : s.tags) std::cout << t << " ";
    std::cout << "\n";
  }
}

void SnipperApp::listSnippetsByTags(const std::string& tagCsv) {
  auto all = _store.loadSnippets();
  std::set<std::string> filterTags;
  std::stringstream ss(tagCsv);
  std::string tag;
  while (std::getline(ss, tag, ',')) {
    filterTags.insert(tag);
  }

  std::cout << "Snippets with tags: " << tagCsv << "\n";
  for (const auto& s : all) {
    bool hasMatch = std::any_of(s.tags.begin(), s.tags.end(),
      [&](const std::string& t){ return filterTags.count(t); });
    if (hasMatch) {
      std::cout << "- " << s.id << " | " << s.title << " | tags: ";
      for (const auto& t : s.tags) std::cout << t << " ";
      std::cout << "\n";
    }
  }
}

void SnipperApp::editSnippet(const std::string& id) {
  auto all = _store.loadSnippets();
  std::string normId = normalizeId(id);

  auto it = std::find_if(all.begin(), all.end(), [&](const model::Snippet& s) {
    return normalizeId(s.id) == normId;
  });

  if (it == all.end()) {
    std::cout << "Snippet not found.\n";
    return;
  }

  model::Snippet& s = *it;
  std::cout << "Editing snippet: " << s.id << " | " << s.title << "\n";

  std::string input;

  std::cout << "Enter new title (or leave blank to keep: \"" << s.title << "\"): ";
  std::getline(std::cin, input);
  if (!input.empty()) s.title = input;

  std::cout << "Enter new content (or leave blank to keep current): ";
  std::getline(std::cin, input);
  if (!input.empty()) s.content = input;

  std::cout << "Current tags: ";
  for (const auto& t : s.tags) std::cout << t << " ";
  std::cout << "\nDo you want to modify tags? (y/n): ";
  std::getline(std::cin, input);
  if (input == "y" || input == "Y") {
    std::cout << "Enter tags to add (comma-separated): ";
    std::getline(std::cin, input);
    std::set<std::string> tagSet(s.tags.begin(), s.tags.end());

    // Add new tags
    if (!input.empty()) {
      std::stringstream ss(input);
      std::string tag;
      while (std::getline(ss, tag, ',')) {
        if (!tag.empty()) tagSet.insert(tag);
      }
    }

    std::cout << "Enter tags to remove (comma-separated): ";
    std::getline(std::cin, input);
    if (!input.empty()) {
      std::stringstream ss(input);
      std::string tag;
      while (std::getline(ss, tag, ',')) {
        if (!tag.empty()) tagSet.erase(tag);
      }
    }

    s.tags.assign(tagSet.begin(), tagSet.end());
  }

  s.updatedAt = getCurrentTimestamp();
  _store.saveSnippets(all);
  std::cout << "Snippet updated.\n";
}

void SnipperApp::exportSnippets(const std::optional<std::string>& idOpt, const std::string& filePath) {
  auto all = _store.loadSnippets();
  std::vector<model::Snippet> toExport;

  if (idOpt.has_value()) {
    std::string norm = normalizeId(idOpt.value());
    auto it = std::find_if(all.begin(), all.end(), [&](const model::Snippet& s) {
      return normalizeId(s.id) == norm;
    });
    if (it != all.end()) {
      toExport.push_back(*it);
    } else {
      std::cout << "Snippet not found.\n";
      return;
    }
  } else {
    toExport = all;
  }

  // Convert to JSON using overloaded to_json
  nlohmann::json j = toExport;

  std::ofstream out(filePath);
  if (!out) {
    std::cout << "Failed to open file: " << filePath << "\n";
    return;
  }

  out << j.dump(2);
  std::cout << "Exported to " << filePath << "\n";
}

void SnipperApp::importSnippets(const std::string& filePath, bool overwrite) {
  std::ifstream in(filePath);
  if (!in) {
    std::cout << "Failed to open file: " << filePath << "\n";
    return;
  }

  nlohmann::json j;
  try {
    in >> j;
  } catch (const std::exception& e) {
    std::cout << "Failed to parse JSON: " << e.what() << "\n";
    return;
  }

  if (!j.is_array()) {
    std::cout << "Invalid format: Expected an array of snippets.\n";
    return;
  }

  auto existingSnippets = _store.loadSnippets();
  std::unordered_map<std::string, model::Snippet> snippetMap;
  for (const auto& s : existingSnippets) {
    snippetMap[normalizeId(s.id)] = s;
  }

  int imported = 0, skipped = 0, replaced = 0;

  for (const auto& item : j) {
    try {
      model::Snippet s = item.get<model::Snippet>();
      std::string normId = normalizeId(s.id);

      if (snippetMap.count(normId)) {
        if (overwrite) {
          _store.removeSnippet(normId);  // Remove the old snippet
          _store.addSnippet(s);          // Add new one
          replaced++;
        } else {
          skipped++;
        }
      } else {
        _store.addSnippet(s);
        imported++;
      }
    } catch (const std::exception& e) {
      std::cout << "Skipping invalid snippet: " << e.what() << "\n";
      skipped++;
    }
  }

  std::cout << "Imported: " << imported
            << " | Replaced: " << replaced
            << " | Skipped: " << skipped << "\n";
}

void SnipperApp::initStore(const std::optional<std::string>& nameOpt) {
  std::string dbPath;

  if (nameOpt) {
    // Ensure the new store is inside the data directory
    dbPath = std::string(DEFAULT_PROJECT_ROOT) + "/data/" + nameOpt.value() + ".db";
  } else {
    dbPath = _config.snippetsDbPath();  // Defaults to DEFAULT_SNIPPETS_DB
  }

  _store = core::SnippetStore(dbPath);

  // Create an empty JSON array in the file
  std::ofstream out(dbPath);
  if (!out) {
    std::cout << "Failed to create store at: " << dbPath << "\n";
    return;
  }

  out << "[]\n";
  std::cout << "Initialized snippet store at: " << dbPath << "\n";
}

void SnipperApp::renameSnippet(const std::string& oldId, const std::string& newId) {
  auto all = _store.loadSnippets();
  const auto oNorm = normalizeId(oldId);
  auto it = std::find_if(all.begin(), all.end(),
                         [&](auto& s){ return normalizeId(s.id) == oNorm; });
  if (it == all.end()) {
    std::cout << "No snippet with ID " << oldId << "\n";
    return;
  }
  it->id = normalizeId(newId);
  _store.saveSnippets(all);
  std::cout << "Renamed snippet " << oldId << " → " << it->id << "\n";
}

void SnipperApp::sortSnippets() {
  auto all = _store.loadSnippets();
  std::sort(all.begin(), all.end(), [](auto& a, auto& b){
    return a.title < b.title;
  });
  _store.saveSnippets(all);
  std::cout << "Snippets sorted by title.\n";
  listSnippets();
}

void SnipperApp::clearSnippets() {
  std::cout << "Are you sure you want to delete ALL snippets? (y/N): ";
  std::string ans;
  std::getline(std::cin, ans);
  if (ans == "y" || ans == "Y") {
    // Save empty list
    _store.saveSnippets({});
    std::cout << "All snippets cleared.\n";
  } else {
    std::cout << "Operation cancelled.\n";
  }
}

void SnipperApp::showStats() {
  auto all = _store.loadSnippets();
  std::cout << "Total snippets: " << all.size() << "\n";
  std::unordered_map<std::string,int> tagCount;
  for (auto& s : all)
    for (auto& t : s.tags)
      tagCount[t]++;
  if (!tagCount.empty()) {
    std::cout << "Tag usage:\n";
    for (auto& [tag,c] : tagCount)
      std::cout << "  " << tag << ": " << c << "\n";
  }
}

}  // namespace app
