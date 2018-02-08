#pragma once
#include <memory>
#include "UIElement.h"
#include "Sprite.h"


class Button : public UIElement
{
public:
	Button(SDL_Rect rect, PState state, const std::string& name);
	bool isClicked(const SDL_MouseButtonEvent e, PState& state) override;
	void render() override;
private:
	std::shared_ptr<Sprite> sprite;
	std::string name;
};

