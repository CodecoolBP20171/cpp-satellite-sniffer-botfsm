#pragma once
#include <vector>
#include <SDl.h>
#include <CoordGeodetic.h>
class Satellite;
class Trajectory
{
public:
	enum Direction {
		FORWARD = 1,
		BACK = -1
	};
	Trajectory(Satellite& sat, Direction direction);
	void calculate(std::time_t time);
	void render();

	void renderSegment(const Sint16 &x1, const Sint16 &y1, const Sint16 &x2, const Sint16 &y2);

private:
	Satellite& sat;
	Direction direction;
	std::vector<CoordGeodetic> points;
	double Trajectory::getDistance(const CoordGeodetic & a, const CoordGeodetic & b);
};

