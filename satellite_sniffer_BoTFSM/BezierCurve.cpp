#include "stdafx.h"
#include "BezierCurve.h"
#include "SDL_PointOperators.hpp"

BezierCurve::BezierCurve(SDL_Point & p0,
						 SDL_Point & p1,
						 SDL_Point & p2,
						 SDL_Point & p3) :
	p0(p0), p1(p1), p2(p2), p3(p3)
{
}

void BezierCurve::render()
{

}

SDL_Point BezierCurve::calculatePoint(double t)
{
	double u = 1 - t;
	double tsquare = t * t;
	double usquare = u * u;

	SDL_Point p = usquare * u * p0;
	p += 3 * usquare * t * p1;
	p += 3 * u * tsquare * p2;
	p += tsquare * t * p3;

	return p;
}
