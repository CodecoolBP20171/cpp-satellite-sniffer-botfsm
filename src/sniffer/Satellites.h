#pragma once
#include "Config.h"
#include "Satellite.h"
#include "UISatellite.h"

#include <list>
#include <memory>
#include <vector>

template <typename T> using sptr = std::shared_ptr<T>;
class Satellites {
public:
  static sptr<Satellites> &getInstance();
  void renderUISatellits(int zoom);
  void renderPopupSatellits();
  void saveSatelliteList();
  void updatePosition();

private:
  static sptr<Satellites> instance;
  Config &mConf;
  void loadSatellites();
  Satellites();
  std::vector<Satellite> sats;
  std::list<UISatellite> UISats;
};
