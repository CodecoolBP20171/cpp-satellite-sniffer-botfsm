#pragma once
#include <vector>
#include <memory>
#include <SDL.h>
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
	void render(int zoom);

private:
	bool isTextureValid;
	SDL_Rect rect;
	Canvas texture;
	Direction direction;
	std::vector<CoordGeodetic> points;
	double getDistance(const CoordGeodetic & a, const CoordGeodetic & b);
	void updateRect(int zoom);
	void renderNewTexture(int zoom);
	void renderSegment(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, int zoom);
	int lastZoom;
};

