#pragma once
#include <SDL.h>

SDL_Point& operator+=(SDL_Point& point1, const SDL_Point& point2) {
	point1.x += point2.x;
	point1.y += point2.y;
	return point1;
}

SDL_Point operator*(const SDL_Point& point, double scalar) {
	return { static_cast<int>(round(point.x * scalar)), static_cast<int>(round(point.y * scalar)) };
}

SDL_Point operator*(double scalar, const SDL_Point& point) {
	return { static_cast<int>(round(point.x * scalar)), static_cast<int>(round(point.y * scalar)) };
}