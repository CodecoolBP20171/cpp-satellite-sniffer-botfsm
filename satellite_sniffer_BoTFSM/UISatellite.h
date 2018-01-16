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
	bool isClicked(const int x, const int y, PState& state) override;
	void popupRender();
private:
	static int pos;
	Satellite & sat;
	std::shared_ptr<Sprite> texture;
	std::shared_ptr<Sprite> trajectoryForward;
	std::shared_ptr<Sprite> trajectoryBackward;
	std::unique_ptr<ScreenText> text;
	std::unique_ptr<ScreenText> popupText;
	void renderTrajectory();
	void renderPoint(const CoordGeodetic& coord, std::shared_ptr<Sprite>& point);
	double getDistance(const CoordGeodetic& a, const CoordGeodetic& b);
};

