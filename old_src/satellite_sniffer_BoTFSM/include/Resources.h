#pragma once

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

  void operator()(SDL_Renderer *p) const { SDL_DestroyRenderer(p); } // not needed anymore

  void operator()(SDL_Texture *p) const { SDL_DestroyTexture(p); }

  void operator()(TTF_Font *p) const { TTF_CloseFont(p); }
};
class Resources {
public:
  struct rgb_color {
    float r, g, b;
  };
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
  SDL_Rect getMapDimensions();

  SDL_Window *getWindow();
  SDL_GLContext &getGLContext();
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
  rgb_color trajectoryForwardColor;
  rgb_color trajectoryBackwardColor;
  SDL_Rect mapDimensions{0,0,0,0};
  SDL_Rect iconDimensions{0,0,0,0};

private:
  static sptr<Resources> instance;

  std::unique_ptr<SDL_Window, sdl_deleter> window;
  std::unique_ptr<TTF_Font, sdl_deleter> ttffont;
  SDL_GLContext glContext;

  std::map<std::string, std::array<float, 8>> atlasCoords;

  void release();

  Resources();
  void loadTextures();
  void initAtlas();
  void loadShaders();
  void initBuffers();
};
