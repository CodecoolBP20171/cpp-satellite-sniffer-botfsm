#pragma once
#include <ctime>
#include <vector>
#include "GeoCoordinate.h"
#include "Sprite.h"
#include "ScreenText.h"
#include "Trajectory.h"

#include <SGP4.h>

class Satellite {
public:
	static Tle loadTle(const std::string& name, const std::string& noradId);
	Satellite(std::string name, std::string noradId, std::string type, bool visible = true);
	void transformOrigo();
	void updatePosition();
	void updatePosition(std::time_t time);
	CoordGeodetic getPositionAtTime(std::time_t& time);
	CoordGeodetic& getPosition();
	Trajectory& getForwardTrajectory();
	Trajectory& getBackTrajectory();

	~Satellite();
	void toggleShown();
	time_t getDelta(std::time_t& time);
	bool isShown();
	void show();
	void hide();
	std::string& getName();
	std::string& getType();
	std::string toString();
private:
	Tle tle;
	SGP4 sgp4;
	std::string name, noradId, type;
	CoordGeodetic satpos;
	Trajectory forwardTrajectory, backTrajectory;
	bool _shown;
	void calculate(std::tm& time);
};

