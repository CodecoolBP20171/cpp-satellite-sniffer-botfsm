// satellite_sniffer_BoTFSM.cpp : Defines the entry point for the console
// application.

#include "stdafx.h"

#include "LoadError.hpp"
#include "Program.h"


#include <iostream>

int main(int argc, char *argv[]) {
  Program p;
  try {
    p.init();
    p.run();
  } catch (LoadError &e) {
    std::cout << e.what() << std::endl;
    std::cin.get();
  }
  p.unload();
  return 0;
}
