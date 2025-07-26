#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <core/SnippetStore.h>
#include <models/Snippet.h>

int main() {
  std::cout << "[test_snippet_store] running...\n";
  const std::string db = "tmp.db";
  std::filesystem::remove(db);

  core::SnippetStore store(db);
  auto list0 = store.loadSnippets();
  assert(list0.empty());

  model::Snippet s;
  s.id = "1";
  s.title = "T";
  s.content = "C";
  s.tags = {"a","b"};
  s.createdAt = s.updatedAt = "2025-01-01T00:00:00";

  store.addSnippet(s);
  auto list1 = store.loadSnippets();
  assert(list1.size() == 1);
  assert(list1[0].id == "1");

  // JSON round-trip
  auto j = list1[0].toJson();
  auto s2 = model::Snippet::fromJson(j);
  assert(s2.id == s.id);

  store.removeSnippet("1");
  auto list2 = store.loadSnippets();
  assert(list2.empty());

  std::filesystem::remove(db);
  std::cout << "[test_snippet_store] passed.\n";
  return 0;
}
