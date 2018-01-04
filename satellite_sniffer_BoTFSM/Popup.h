#pragma once
#include <memory>
#include <list>

#include "UIElement.h"
#include "UISatellite.h"
#include "Button.h"
#include "Satellite.h"

class Popup : public UIElement
{
public:
	Popup(SDL_Rect rect, PState state, std::list<UISatellite>& satList);
	bool isClicked(const int x, const int y, PState& state) override;
	void render() override;
private:
	std::unique_ptr<Button> button;
	std::list<UISatellite>& sats;
};

