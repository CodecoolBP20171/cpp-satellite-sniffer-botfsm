#pragma once
#include <utility>
#include <list>
#include "Texture.h"
#include <ctime>
#include <SDL.h>
#include <SGP4.h>

class Satellite {
public:
	static Tle loadTle(const std::string& name, const std::string& noradId);
	Satellite(std::string name, std::string noradId, std::string type);
	std::pair<double, double> calculate(std::tm& time);
	std::pair<double, double> calculate();
	void render(SDL_Rect& mapSize, std::time_t time = 0);
	~Satellite();
private:
	SGP4 sgp4;
	std::string name, noradId, type;
	std::shared_ptr<Texture> texture;
};

