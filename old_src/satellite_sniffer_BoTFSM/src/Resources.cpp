#include "Resources.h"

#include "Config.h"
#include "LoadError.hpp"
#include "stdafx.h"

#include <SDL_image.h>

#include <fstream>
#include <iostream>
#include <vector>

namespace {
static GLuint loadShader(GLenum _shaderType, const std::string &_fileName) {
  GLuint loadedShader = glCreateShader(_shaderType);

  if (loadedShader == 0) { throw LoadError(); }

  std::string shaderCode = "";
  std::ifstream shaderStream(_fileName);

  if (!shaderStream.is_open()) { throw LoadError(); }

  std::string line;
  while (std::getline(shaderStream, line)) { shaderCode += line + "\n"; }

  shaderStream.close();

  const char *sourcePointer = shaderCode.c_str();
  glShaderSource(loadedShader, 1, &sourcePointer, NULL);

  glCompileShader(loadedShader);

  GLint result = GL_FALSE;
  int infoLogLength;

  glGetShaderiv(loadedShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(loadedShader, GL_INFO_LOG_LENGTH, &infoLogLength);

  if (GL_FALSE == result) {
    std::vector<char> VertexShaderErrorMessage(infoLogLength);
    glGetShaderInfoLog(loadedShader, infoLogLength, NULL, &VertexShaderErrorMessage[0]);

    std::cerr << "Shader compilation error in file: " << _fileName << '\n';
    std::cerr << &VertexShaderErrorMessage[0] << '\n';
  }

  return loadedShader;
}
} // namespace

sptr<Resources> Resources::instance = nullptr;
Resources::Resources() {
  std::unique_ptr<SDL_Window, sdl_deleter> win(SDL_CreateWindow("Satellites",
                                                                SDL_WINDOWPOS_CENTERED,
                                                                SDL_WINDOWPOS_CENTERED,
                                                                Config::getIntOption("Dimensions", "WINDOW_WIDTH"),
                                                                Config::getIntOption("Dimensions", "WINDOW_HEIGHT"),
                                                                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL));
  if (!win) {
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    throw LoadError();
  }
  win.swap(window);
  glContext = SDL_GL_CreateContext(window.get());

  GLenum res{0};
  if ((res = glewInit())) {
    std::cerr << "Failed to initialize OpenGL loader!\n" << glewGetErrorString(res) << std::endl;
    throw LoadError();
  }

   int glVersion[2] = {-1, -1};
   glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
   glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
   std::cout << "Running OpenGL " << glVersion[0] << "." << glVersion[1] << std::endl;

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_PRIMITIVE_RESTART);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPrimitiveRestartIndex(static_cast<GLuint>(-1));

  std::unique_ptr<TTF_Font, sdl_deleter> fon(TTF_OpenFont(Config::getStringOption("FontFiles", "MAP_TEXT").c_str(),
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
    GL_CHECK;
    instance->loadTextures();
    GL_CHECK;
    instance->initAtlas();
    GL_CHECK;
    instance->loadShaders();
    GL_CHECK;
    instance->initBuffers();
    GL_CHECK;
  }
  return instance;
}

void Resources::loadTextures() {
  auto *mapSurface = IMG_Load(Config::getStringOption("TextureFiles", "MAP").c_str());
  if (nullptr == mapSurface) { throw LoadError(); }
  glGenTextures(1, &mapTextureId);
  glBindTexture(GL_TEXTURE_2D, mapTextureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               mapSurface->w,
               mapSurface->h,
               0,
               mapSurface->format->BytesPerPixel == 3 ? GL_RGB : GL_RGBA,
               GL_UNSIGNED_BYTE,
               mapSurface->pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  mapDimensions.w = mapSurface->w;
  mapDimensions.h = mapSurface->h;
  SDL_FreeSurface(mapSurface);

  auto *atlasSurface = IMG_Load(Config::getStringOption("TextureFiles", "ATLAS").c_str());
  if (nullptr == atlasSurface) { throw LoadError(); }
  glGenTextures(1, &atlasTextureId);
  glBindTexture(GL_TEXTURE_2D, atlasTextureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               atlasSurface->w,
               atlasSurface->h,
               0,
               atlasSurface->format->BytesPerPixel == 3 ? GL_RGB : GL_RGBA,
               GL_UNSIGNED_BYTE,
               atlasSurface->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7);
  glGenerateMipmap(GL_TEXTURE_2D);
  const int N = 4; // NxN entries in atlas
  iconDimensions.w = atlasSurface->w / N;
  iconDimensions.h = atlasSurface->h / N;
  SDL_FreeSurface(atlasSurface);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Resources::initAtlas() {
  static const char *NAMES[] = {"STATION", "GPS", "TELESCOPE"};
  static const int ARRAY_SIZE = sizeof(NAMES) / sizeof(NAMES[0]);
  const int N = 4; // NxN entries in the atlas
  const float N_INV = 1.f / N;
  for (int i = 0; i < ARRAY_SIZE; i++) {
    float xMin = N_INV * (i % N), xMax = xMin + N_INV, yMin = N_INV * (i / N), yMax = yMin + N_INV;
    atlasCoords.emplace(
        std::make_pair<std::string, std::array<float, 8>>(NAMES[i], {xMin, yMax, xMax, yMax, xMin, yMin, xMax, yMin}));
  }
}

void Resources::loadShaders() {
  GLuint vs_ID = loadShader(GL_VERTEX_SHADER, "textureVert.vert");
  GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, "textureFrag.frag");

  textureProgramId = glCreateProgram();

  glAttachShader(textureProgramId, vs_ID);
  glAttachShader(textureProgramId, fs_ID);

  glLinkProgram(textureProgramId);

  vs_ID = loadShader(GL_VERTEX_SHADER, "trajectoryVert.vert");
  fs_ID = loadShader(GL_FRAGMENT_SHADER, "trajectoryFrag.frag");

  trajectoryProgramId = glCreateProgram();

  glAttachShader(trajectoryProgramId, vs_ID);
  glAttachShader(trajectoryProgramId, fs_ID);

  glLinkProgram(trajectoryProgramId);
}

void Resources::initBuffers() {

  // map buffers

  texture_vertex verticies[] = {{0.f, 1.f, 0.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, {0.f, 0.f, 0.f, 0.f}, {1.f, 0.f, 1.f, 0.f}};
  GLuint indices[] = {0, 1, 2, 2, 1, 3};

  glGenVertexArrays(1, &mapVAOId);
  glBindVertexArray(mapVAOId);

  glGenBuffers(1, &mapVBOId);
  glBindBuffer(GL_ARRAY_BUFFER, mapVBOId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(texture_vertex), 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(texture_vertex), (void *)(2 * sizeof(float)));

  glGenBuffers(1, &mapIBOId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mapIBOId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // icon buffers
  glGenVertexArrays(1, &iconVAOId);
  glBindVertexArray(iconVAOId);

  glGenBuffers(1, &iconVBOId);
  glBindBuffer(GL_ARRAY_BUFFER, iconVBOId);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(texture_vertex), 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(texture_vertex), (void *)(2 * sizeof(float)));

  glGenBuffers(1, &iconIBOId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iconIBOId);

  // trajectory buffers
  glGenVertexArrays(1, &trajectoryVAOId);
  glBindVertexArray(trajectoryVAOId);

  glGenBuffers(1, &trajectoryVBOId);
  glBindBuffer(GL_ARRAY_BUFFER, trajectoryVBOId);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(color_vertex), 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(color_vertex), (void *)(2 * sizeof(float)));

  glGenBuffers(1, &trajectoryIBOId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trajectoryIBOId);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

SDL_Window *Resources::getWindow() { return window.get(); }

SDL_GLContext &Resources::getGLContext() { return glContext; }

void Resources::releaseResources() {
  if (instance) instance->release();
  instance.reset();
}

TTF_Font *Resources::getFont() { return ttffont.get(); }

void Resources::release() {
  ttffont.reset();
  SDL_GL_DeleteContext(glContext);
  window.reset();
}

bool Resources::GLErrors(const char *file, int line) {
  GLenum err = glGetError();
  const char *msg;
  const char *expl;
  switch (err) {
  case GL_NO_ERROR:
    return true;
    break; // returns true if ok!
  case GL_INVALID_ENUM:
    msg = "Invalid enum";
    expl = "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no "
           "other side effect than to set the error flag.";
    break;
  case GL_INVALID_VALUE:
    msg = "Invalid value";
    expl = "A numeric argument is out of range.The offending command is ignored and has no other side effect than to "
           "set the error flag.";
    break;
  case GL_INVALID_OPERATION:
    msg = "Invalid operation";
    expl = "The specified operation is not allowed in the current state. The offending command is ignored and has no "
           "other side effect than to set the error flag.";
    break;
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    msg = "Invalid framebuffer operation";
    expl = "The framebuffer object is not complete. The offending command is ignored and has no other side effect than "
           "to set the error flag.";
    break;
  case GL_OUT_OF_MEMORY:
    msg = "Out of memory";
    expl = "There is not enough memory left to execute the command. The state of the GL is undefined, except for the "
           "state of the error flags, after this error is recorded.";
    break;
  case GL_STACK_UNDERFLOW:
    msg = "Stack underflow";
    expl = "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
    break;
  case GL_STACK_OVERFLOW:
    msg = "Stack overflow";
    expl = "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
    break;
  default:
    msg = "Unknown error";
    expl = "glGetError() returned an unknown value. Weird.";
    break;
  }                                                            // USE_COLOR_CODES
  std::cerr << "OPENGL ERROR " << file << ":" << line << "\n"; // USE_COLOR_CODES
  std::cerr << msg << ": " << expl << std::endl;
  return false;
}