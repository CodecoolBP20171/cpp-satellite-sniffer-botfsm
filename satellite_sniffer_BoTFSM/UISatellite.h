#pragma once
#include <memory>
#include <ctime>
#include "Sprite.h"
#include "ScreenText.h"
#include "UIElement.h"
#include "Satellite.h"

class UISatellite : UIElement
{
public:
	UISatellite(Satellite& sat);
	void render() override;
	void popupRender(int pos);
private:
	Satellite & sat;
	std::shared_ptr<Sprite> texture;
	std::shared_ptr<Sprite> trajectoryForward;
	std::shared_ptr<Sprite> trajectoryBackward;
	std::unique_ptr<ScreenText> text;
	std::unique_ptr<ScreenText> popupText;
	void renderTrajectory();
	void renderPoint(std::time_t& now, std::shared_ptr<Sprite>& point);
};

