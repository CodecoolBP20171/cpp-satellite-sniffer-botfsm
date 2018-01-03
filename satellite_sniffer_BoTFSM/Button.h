#pragma once
#include <memory>
#include "UIElement.h"
#include "Sprite.h"


class Button : public UIElement
{
public:
	Button(SDL_Rect rect, const std::string& name);

	void render() override;
private:
	std::shared_ptr<Sprite> sprite;
};

