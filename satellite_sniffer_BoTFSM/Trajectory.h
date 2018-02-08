#pragma once
#include <vector>
#include <memory>
#include <SDl.h>
#include <CoordGeodetic.h>
#include "Sprite.h"
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
	void render(int zoom);

private:
	bool isTextureValid;
	SDL_Rect rect;
	int offSetX, offSetY;
	std::unique_ptr<Sprite> texture;
	Satellite& sat;
	Direction direction;
	std::vector<CoordGeodetic> points;
	double getDistance(const CoordGeodetic & a, const CoordGeodetic & b);
	void updateRect(int zoom);
	void renderNewTexture(int zoom);
	void renderSegment(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, int zoom);
	int lastZoom;
};

