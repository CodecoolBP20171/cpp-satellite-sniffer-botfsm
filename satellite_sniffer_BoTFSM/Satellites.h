#pragma once
#include <memory>
#include <list>
#include "Satellite.h"
#include "UISatellite.h"

template<typename T>
using sptr = std::shared_ptr<T>;
class Satellites
{
public:
	static sptr<Satellites>& getInstance();
	std::list<Satellite>& getSatellites();
	std::list<UISatellite>& getUISatellites();
	void renderUISatellits();
	void renderPopupSatellits();
	void saveSatelliteList();
	void updatePosition();
	~Satellites();
private:
	static sptr<Satellites> instance;
	void loadSatellites();
	Satellites();
	std::list<Satellite> sats;
	std::list<UISatellite> UISats;
};

