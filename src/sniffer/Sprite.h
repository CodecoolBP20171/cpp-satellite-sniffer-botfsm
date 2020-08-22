#pragma once
#include "Texture.h"

#include <memory>
#include <string>
class Sprite : public Texture {
public:
  explicit Sprite(const std::string &fileName);
  explicit Sprite(const std::shared_ptr<Sprite> &other);
  void setAsRenderTarget() override;
  virtual ~Sprite();
};
