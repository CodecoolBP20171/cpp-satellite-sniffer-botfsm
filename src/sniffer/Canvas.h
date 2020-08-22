#pragma once
#include "Texture.h"
class Canvas : public Texture {
public:
  explicit Canvas(const int width = 0, const int height = 0);
  void resize(const int width, const int height);
  void setAsRenderTarget() override;
  virtual ~Canvas();
};
