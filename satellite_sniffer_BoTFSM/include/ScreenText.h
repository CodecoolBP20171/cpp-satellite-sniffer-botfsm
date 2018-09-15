#pragma once
#include <string>
#include "Texture.h"

class ScreenText : public Texture {
public:
	ScreenText(std::string& text);
	virtual ~ScreenText();
	void setAsRenderTarget() override;
	void setColor(SDL_Color color);
};

