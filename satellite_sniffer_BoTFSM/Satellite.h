#pragma once
#include <utility>
#include <list>
#include <ctime>
#include <vector>
#include "GeoCoordinate.h"
#include "Texture.h"
#include "ScreenText.h"

#include <SGP4.h>

class Satellite {
public:
	static Tle loadTle(const std::string& name, const std::string& noradId);
	Satellite(std::string name, std::string noradId, std::string type);
	void setPosition(std::time_t time = 0);
	void render();
	~Satellite();
private:
	SGP4 sgp4;
	std::string name, noradId, type;
	std::shared_ptr<Texture> texture;
	std::vector<std::shared_ptr<Texture>> pointsForward;
	std::vector<std::shared_ptr<Texture>> pointsBackward;
	std::unique_ptr<ScreenText> text;
	GeoCoordinate satpos;
	void calculate(std::tm& time);
	void calculate();
	void renderTrajectory();
	void renderPoint(std::time_t& now, std::shared_ptr<Texture>& point);
};

