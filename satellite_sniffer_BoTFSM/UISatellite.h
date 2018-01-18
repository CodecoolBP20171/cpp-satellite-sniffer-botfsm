#pragma once
#include <memory>
#include <ctime>
#include "Sprite.h"
#include "ScreenText.h"
#include "UIElement.h"
#include "Satellite.h"
#include "Trajectory.h"

class UISatellite : UIElement
{
public:
	UISatellite(Satellite& sat);
	void render() override;
	bool isClicked(const int x, const int y, PState& state) override;
	void popupRender();
	std::string toString();
private:
	static int pos;
	Satellite & sat;
	std::shared_ptr<Sprite> texture;
	std::unique_ptr<ScreenText> text;
	std::unique_ptr<ScreenText> popupText;
	Trajectory& trajectoryForward;
	Trajectory& trajectoryBackward;
	void renderTrajectory();
};

