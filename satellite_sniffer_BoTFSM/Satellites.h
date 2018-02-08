#pragma once
#include <memory>
#include <list>
#include <vector>
#include "Satellite.h"
#include "UISatellite.h"

template<typename T>
using sptr = std::shared_ptr<T>;
class Satellites
{
public:
	static sptr<Satellites>& getInstance();
	std::list<UISatellite>& getUISatellites();
	void renderUISatellits(int zoom);
	void renderPopupSatellits();
	void saveSatelliteList();
	void updatePosition();
private:
	static sptr<Satellites> instance;
	void loadSatellites();
	Satellites();
	std::vector<Satellite> sats;
	std::list<UISatellite> UISats;
};

