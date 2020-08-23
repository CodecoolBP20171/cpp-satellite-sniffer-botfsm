#pragma once
#include <vector>
#include <memory>
#include <SDL.h>
#include <CoordGeodetic.h>
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
	Direction direction;
	std::vector<CoordGeodetic> points;
	std::vector<float> drawPoints; // coordinate pairs normalized to 0..1
	double getDistance(const CoordGeodetic & a, const CoordGeodetic & b);
	void renderNewTexture(int zoom);
	int lastZoom;
};

