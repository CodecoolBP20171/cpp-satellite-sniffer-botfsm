#include "Program.h"

#include "Config.h"
#include "Map.h"
#include "Menu.h"
#include "Popup.h"
#include "Resources.h"
#include "Satellites.h"
#include "stdafx.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <GL/gl.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

Program::Program()
    : quit(false), loaded(false), timePassed(0), timestep(1000 / 30), // frame time length 1000 / 60
      lastCalculationTime(0), calculationTimeStep(5000),              // 5 sec
      zoom(Config::getIntOption("ZoomLevel", "MIN")), state(PState::RUNNING) {}

Program::~Program() {
  if (loaded) unload();
}

void Program::init() {
  // init SDL
  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG);
  TTF_Init();
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  // init resources
  Resources::getInstance();
  Satellites::getInstance();

  UIElements.emplace_back(std::make_unique<Map>(Config::getRect("MAP"), PState::RUNNING, zoom));
  UIElements.emplace_back(std::make_unique<Menu>(Config::getRect("MENU"), PState::RUNNING, state));
  UIElements.emplace_back(std::make_unique<Popup>(Config::getRect("POPUP"), PState::PAUSED, state));

  IMGUI_CHECKVERSION();
  ig_context = ImGui::CreateContext();
  io = &ImGui::GetIO();
  // Enable Keyboard
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // Enable Gamepad Controls
  // Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForOpenGL(Resources::getInstance()->getWindow(), Resources::getInstance()->getGLContext());
  ImGui_ImplOpenGL3_Init(/*glsl_version=*/"#version 130");

  loaded = true;
}

void Program::run() {
  if (!loaded) init();
  while (!quit) {
    timePassed = SDL_GetTicks();
    quit = handleEvents();
    updatePositions();

    render();

    /* wait for next frame */
    while (timePassed + timestep > SDL_GetTicks()) { SDL_Delay(0); }
  }
}

void Program::unload() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext(ig_context);
  Resources::releaseResources();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  loaded = false;
}

void Program::updatePositions() {
  if (state != PState::PAUSED && timePassed > lastCalculationTime + calculationTimeStep) {
    Satellites::getInstance()->updatePosition();
    lastCalculationTime = timePassed;
  }
}

void Program::render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(Resources::getInstance()->getWindow());
  ImGui::NewFrame();

  glClearColor(50 / 255.f, 50 / 255.f, 50 / 255.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  for (auto &elem : UIElements) {
    if (elem->isActive(state)) { elem->render(); }
  }

  // ImGui::ShowDemoWindow();
  // ImGui::ShowMetricsWindow();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(Resources::getInstance()->getWindow());
}

bool Program::handleEvents() {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    ImGui_ImplSDL2_ProcessEvent(&e);
    if (e.type == SDL_QUIT) { return true; }
    auto &io = ImGui::GetIO();
    if (!io.WantCaptureMouse) {
      if (e.type == SDL_MOUSEBUTTONUP) {
        for (auto &elem : UIElements) {
          if (elem->isActive(state) && elem->isClicked(e.button, state)) { break; }
        }
      }
    }
    if (PState::QUIT == state) { return true; }
  }
  return false;
}
