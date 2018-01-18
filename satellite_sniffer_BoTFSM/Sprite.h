#pragma once
#include <string>
#include <memory>
#include "Texture.h"
class Sprite : public Texture {
public:
	explicit Sprite(const std::string& fileName);
	explicit Sprite(const std::shared_ptr<Sprite>& other);
	explicit Sprite(const int width, const int height);
	void setAsRenderTarget() override;
};

