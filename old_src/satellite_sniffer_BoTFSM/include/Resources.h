#pragma once
#include "Sprite.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <GL/glew.h>

#include <map>
#include <memory>
#include <vector>

#define GL_CHECK Resources::GLErrors(__FILE__, __LINE__)

template <typename T> using sptr = std::shared_ptr<T>;
struct sdl_deleter {
  void operator()(SDL_Window *p) const { SDL_DestroyWindow(p); }

  void operator()(SDL_Renderer *p) const { SDL_DestroyRenderer(p); }

  void operator()(SDL_Texture *p) const { SDL_DestroyTexture(p); }

  void operator()(TTF_Font *p) const { TTF_CloseFont(p); }
};
class Resources {
public:
  struct texture_vertex {
    float posx, posy, texx, texy;
  };
  struct color_vertex {
    float posx, posy, colr, colg, colb;
  };

  static bool GLErrors(const char *file, int line);
  static sptr<Resources> &getInstance();
  static void releaseResources();
  TTF_Font *getFont();
  sptr<Sprite> &getMap();
  sptr<Sprite> &getCleanMap();
  SDL_Rect getMapDimensions();
  sptr<Sprite> &getSat(std::string &type);

  SDL_Window *getWindow();
  SDL_Renderer *getRenderer();
  SDL_GLContext &getGLContext();
  void resetRenderer();
  GLuint textureProgramId;
  GLuint trajectoryProgramId;
  GLuint mapTextureId;
  GLuint atlasTextureId;
  GLuint mapVBOId, mapVAOId, mapIBOId;
  GLuint iconVBOId, iconVAOId, iconIBOId;
  GLuint trajectoryVBOId, trajectoryVAOId, trajectoryIBOId;
  std::vector<texture_vertex> iconBuffer;
  std::vector<GLuint> iconIndexBuf;
  std::vector<color_vertex> trajectoryBuffer;
  std::vector<GLuint> trajectoryIndexBuf;
  float zcx{.5f}, zcy{.5f};

private:
  static sptr<Resources> instance;

  std::unique_ptr<SDL_Window, sdl_deleter> window;
  std::unique_ptr<SDL_Renderer, sdl_deleter> renderer;
  std::unique_ptr<TTF_Font, sdl_deleter> ttffont;
  SDL_GLContext glContext;

  sptr<Sprite> map;
  sptr<Sprite> cleanMap;
  std::map<std::string, sptr<Sprite>> sats;
  std::map<std::string, std::array<float, 8>> atlasCoords;

  void release();

  Resources();
  void loadTextures();
  void initAtlas();
  void loadShaders();
  void initBuffers();
};
