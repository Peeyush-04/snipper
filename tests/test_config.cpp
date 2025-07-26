#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <app/ConfigManager.h>

int main(int argc, char** argv) {
  std::cout << "[test_config] running...\n";

  // 1. Default config
  app::ConfigManager cfg1(1, argv);
  auto path = cfg1.snippetsDbPath();
  assert(path.find("data/snippets.db") != std::string::npos);
  auto& hot = cfg1.hotkeys();
  assert(hot.at("openPalette") == "Ctrl+Shift+S");
  assert(hot.at("copySnippet") == "Ctrl+Shift+C");
  auto& ui = cfg1.uiPrefs();
  assert(ui.theme == "light");
  assert(ui.fontFamily == "Consolas");
  assert(ui.fontSize == 12);

  // 2. Override
  const char* tmpJson = R"({"snippetsDbPath":"mydb.json"})";
  const std::string tmpPath = "tmp_cfg.json";
  std::ofstream(tmpPath) << tmpJson;
  char* args2[] = {(char*)"prog", (char*)tmpPath.c_str()};
  app::ConfigManager cfg2(2, args2);
  assert(cfg2.snippetsDbPath().find("mydb.json") != std::string::npos);
  std::filesystem::remove(tmpPath);

  std::cout << "[test_config] passed.\n";
  return 0;
}
