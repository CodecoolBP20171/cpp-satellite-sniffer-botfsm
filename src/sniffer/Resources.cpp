#include "Resources.h"
#include "Config.h"
#include "LoadError.hpp"
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>

sptr<Resources> Resources::instance = nullptr;
Resources::Resources() {
  std::unique_ptr<SDL_Window, sdl_deleter> win(SDL_CreateWindow(
      "Satellites", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      Config::getIntOption("Dimensions", "WINDOW_WIDTH"),
      Config::getIntOption("Dimensions", "WINDOW_HEIGHT"),
      SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL));
  if (!win) {
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    throw LoadError();
  }
  win.swap(window);
  glContext = SDL_GL_CreateContext(window.get());

  // find oGL renderer driver id
  auto driverMax = SDL_GetNumRenderDrivers();
  auto oglDriverId = -1;
  for (int i = 0; i < driverMax; ++i) {
    SDL_RendererInfo info;
    SDL_GetRenderDriverInfo(i, &info);
    if (strstr(info.name, "opengl")) {
      oglDriverId = i;
      break;
    }
  }

  std::unique_ptr<SDL_Renderer, sdl_deleter> ren(
      SDL_CreateRenderer(window.get(), oglDriverId,
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
  if (!ren) {
    std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    throw LoadError();
  }
  ren.swap(renderer);
  GLenum res{0};
  if ((res = glewInit())) {
    std::cout << "Failed to initialize OpenGL loader!" << std::endl;
    std::cout << glewGetErrorString(res) << std::endl;
    throw LoadError();
  }

  std::unique_ptr<TTF_Font, sdl_deleter> fon(
      TTF_OpenFont(Config::getStringOption("FontFiles", "MAP_TEXT").c_str(),
                   Config::getIntOption("Dimensions", "MAP_TEXT_SIZE")));
  if (!fon) {
    std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    throw LoadError();
  }
  fon.swap(ttffont);
}

sptr<Resources> &Resources::getInstance() {
  if (!instance) {
    instance = sptr<Resources>(new Resources());
    instance->loadTextures();
  }
  return instance;
}

void Resources::loadTextures() {
  cleanMap =
      sptr<Sprite>(new Sprite(Config::getStringOption("TextureFiles", "MAP")));
  map = sptr<Sprite>(new Sprite(cleanMap));

  sats.emplace(Config::getStringOption("SatelliteType", "STATION"),
               sptr<Sprite>(new Sprite(
                   Config::getStringOption("TextureFiles", "STATION"))));
  sats.emplace(Config::getStringOption("SatelliteType", "TELESCOPE"),
               sptr<Sprite>(new Sprite(
                   Config::getStringOption("TextureFiles", "TELESCOPE"))));
  sats.emplace(
      Config::getStringOption("SatelliteType", "GPS"),
      sptr<Sprite>(new Sprite(Config::getStringOption("TextureFiles", "GPS"))));
}

SDL_Window *Resources::getWindow() { return window.get(); }

SDL_Renderer *Resources::getRenderer() { return renderer.get(); }

SDL_GLContext &Resources::getGLContext() { return glContext; }

void Resources::resetRenderer() {
  SDL_SetRenderTarget(renderer.get(), nullptr);
}

void Resources::releaseResources() {
  if (instance)
    instance->release();
  instance.reset();
}

TTF_Font *Resources::getFont() { return ttffont.get(); }

sptr<Sprite> &Resources::getMap() { return map; }

sptr<Sprite> &Resources::getCleanMap() { return cleanMap; }

SDL_Rect Resources::getMapDimensions() { return map->getDimensions(); }

sptr<Sprite> &Resources::getSat(std::string &type) { return sats[type]; }

void Resources::release() {
  ttffont.reset();
  map.reset();
  cleanMap.reset();
  for (auto &sat : sats) {
    sat.second.reset();
  }
  renderer.reset();
  SDL_GL_DeleteContext(glContext);
  window.reset();
}