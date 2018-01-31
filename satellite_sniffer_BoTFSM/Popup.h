#pragma once
#include <memory>
#include <list>

#include "UIElement.h"
#include "Button.h"

class Popup : public UIElement
{
public:
	Popup(SDL_Rect rect, PState state);
	bool isClicked(const int x, const int y, PState& state) override;
	void render(PState* state = nullptr) override;
private:
	std::unique_ptr<Button> button;
};

