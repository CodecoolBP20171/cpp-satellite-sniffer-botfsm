#pragma once
#include <ctime>
#include <vector>
#include "GeoCoordinate.h"
#include "Sprite.h"
#include "ScreenText.h"

#include <SGP4.h>

class Satellite {
public:
	static Tle loadTle(const std::string& name, const std::string& noradId);
	Satellite(std::string name, std::string noradId, std::string type);
	void updatePosition(std::time_t time = 0);
	void render();
	~Satellite();
private:
	Tle tle;
	SGP4 sgp4;
	std::string name, noradId, type;
	std::shared_ptr<Sprite> texture;
	std::shared_ptr<Sprite> trajectoryForward;
	std::shared_ptr<Sprite> trajectoryBackward;
	std::unique_ptr<ScreenText> text;
	GeoCoordinate satpos;
	bool _shown;
	void calculate(std::tm& time);
	void calculate();
	void renderTrajectory();
	void renderPoint(std::time_t& now, std::shared_ptr<Sprite>& point);
	GeoCoordinate getPositionAtTime(std::time_t& time);
};

