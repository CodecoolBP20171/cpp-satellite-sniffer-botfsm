#include "LoadError.hpp"
#include "Program.h"

#include <iostream>

int main(int argc, char *argv[]) {
  Program p;
  try {
    p.init();
    p.run();
  } catch (LoadError &e) {
    std::cerr << e.what() << std::endl;
    std::cin.get();
  }
  p.unload();
  return 0;
}
