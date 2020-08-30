#pragma once

#include "Config.h"
#include "types.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <GL/glew.h>

#include <map>
#include <memory>
#include <string_view>
#include <vector>

#define GL_CHECK Resources::GLErrors(__FILE__, __LINE__)

template <typename T> using sptr = std::shared_ptr<T>;
struct sdl_deleter {
  void operator()(SDL_Window *p) const { SDL_DestroyWindow(p); }

  void operator()(SDL_Renderer *p) const { SDL_DestroyRenderer(p); } // not needed anymore

  void operator()(SDL_Texture *p) const { SDL_DestroyTexture(p); }

  void operator()(TTF_Font *p) const { TTF_CloseFont(p); }
};
class Resources {
public:
  static bool GLErrors(const char *file, int line);
  static sptr<Resources> &getInstance();
  static void releaseResources();
  TTF_Font *getFont();

  SDL_Window *getWindow();
  SDL_GLContext &getGLContext();
  const std::array<Graphics::texture_vertex, 4> &getIconTextureVertices(const std::string_view &iconName);
  GLuint textureProgramId;
  GLuint trajectoryProgramId;
  GLuint mapTextureId;
  GLuint atlasTextureId;
  GLuint mapVBOId, mapVAOId, mapIBOId;
  GLuint iconVBOId, iconVAOId, iconIBOId;
  GLuint trajectoryVBOId, trajectoryVAOId, trajectoryIBOId;
  std::vector<Graphics::texture_vertex> iconBuffer;
  std::vector<GLuint> iconIndexBuf;
  std::vector<Graphics::color_vertex> trajectoryBuffer;
  std::vector<GLuint> trajectoryIndexBuf;
  float zcx{.5f}, zcy{.5f};
  SDL_Rect mapDimensions{0, 0, 0, 0};
  SDL_Rect iconDimensions{0, 0, 0, 0};

private:
  static sptr<Resources> instance;

  Config &mConf;
  std::unique_ptr<SDL_Window, sdl_deleter> window;
  std::unique_ptr<TTF_Font, sdl_deleter> ttffont;
  SDL_GLContext glContext;

  std::map<std::string_view, std::array<Graphics::texture_vertex, 4>> atlasCoords;

  void release();

  Resources();
  void loadTextures();
  void initAtlas();
  void loadShaders();
  void initBuffers();
};
