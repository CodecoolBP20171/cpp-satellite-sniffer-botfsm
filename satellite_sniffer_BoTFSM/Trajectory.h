#pragma once
#include <vector>
#include <memory>
#include <SDl.h>
#include <CoordGeodetic.h>
#include "Canvas.h"
class Satellite;
class Trajectory
{
public:
	enum Direction {
		FORWARD = 1,
		BACK = -1
	};
	Trajectory(Direction direction);
	void calculate(std::time_t time, Satellite& sat);
	void render();

private:
	bool isTextureValid;
	SDL_Rect rect;
	int offSetX, offSetY;
	Canvas texture;
	Direction direction;
	std::vector<CoordGeodetic> points;
	double getDistance(const CoordGeodetic & a, const CoordGeodetic & b);
	void updateRect();
	void renderNewTexture();
	void renderSegment(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2);
};

