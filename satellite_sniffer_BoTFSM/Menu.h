#pragma once
#include <memory>
#include <list>
#include "UIElement.h"
#include "Button.h"
#include "Sprite.h"

class Menu : public UIElement
{
public:
	Menu(SDL_Rect rect, PState state);
	bool isClicked(const SDL_MouseButtonEvent e, PState& state) override;
	void render() override;
private:
	std::list<std::shared_ptr<Button>> menuButtons;
};

