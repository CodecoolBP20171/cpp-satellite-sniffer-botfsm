#pragma once
#include "Config.h"
#include "ProgramState.h"
#include "UIElement.h"

#include <memory>
#include <vector>

struct ImGuiIO;
struct ImGuiContext;

class Program {
public:
  Program();
  ~Program();
  void init();
  void run();
  void unload();

private:
  Config &mConf;
  std::vector<std::unique_ptr<UIElement>> UIElements;
  PState state;
  bool quit;
  bool loaded;
  int zoom;
  Uint32 timePassed;
  Uint32 timestep;
  Uint32 lastCalculationTime;
  Uint32 calculationTimeStep;
  ImGuiContext *ig_context = nullptr;
  ImGuiIO *io = nullptr;
  bool handleEvents();
  void updatePositions();
  void render();
};
