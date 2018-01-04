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
	GeoCoordinate getPositionAtTime(std::time_t& time);
	GeoCoordinate& getPosition();

	~Satellite();

	int getDelta();
	bool& isShown();
	std::string& getName();
	std::string& getType();
private:
	Tle tle;
	SGP4 sgp4;
	std::string name, noradId, type;
	GeoCoordinate satpos;
	bool _shown;
	void calculate(std::tm& time);
	void calculate();
};

