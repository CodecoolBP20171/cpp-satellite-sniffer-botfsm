#pragma once
#include <list>

#include "Satellite.h"

class Program {
public:
	Program();
	~Program();
	void init();
	void run();
	void unload();
private:
	std::list<Satellite> sats;
	bool quit;
	bool loaded;
	Uint32 timePassed;
	Uint32 timestep;
	Uint32 lastCalculationTime;
	Uint32 calculationTimeStep;
	bool handleEvents();
	void updatePositions();
	void render();
};

