#pragma once
#include <SDL.h>
class BezierCurve
{
public:
	BezierCurve(SDL_Point& p0,
				SDL_Point& p1,
				SDL_Point& p2,
				SDL_Point& p3);
	void render();
private:
	SDL_Point p0, p1, p2, p3;
	SDL_Point calculatePoint(double t);
};

