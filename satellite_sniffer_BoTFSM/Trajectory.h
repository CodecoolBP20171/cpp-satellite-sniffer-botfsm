#pragma once
#include "Texture.h"
#include "GeoCoordinate.h"

class Trajectory
{
public:
	Trajectory();
	~Trajectory();
private:
	std::shared_ptr<Texture> texture;
	GeoCoordinate trapos;
};

