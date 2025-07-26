#include <app/SnipperApp.h>
#include <iostream>

int main(int argc, char** argv) {
  try {
    app::SnipperApp app(argc, argv);
    app.run();
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }
  return 0;
}
