#pragma once
#include "Config.h"
#include "Trajectory.h"

#include <SGP4.h>

#include <ctime>
#include <vector>

class Satellite {
public:
  Satellite(std::string name, std::string noradId, std::string type, bool visible = true);
  void transformOrigo();
  void updatePosition();
  void updatePosition(std::time_t time);
  CoordGeodetic getPositionAtTime(std::time_t &time);
  CoordGeodetic &getPosition();
  Trajectory &getForwardTrajectory();
  Trajectory &getBackTrajectory();
  ~Satellite();

  void toggleShown();
  time_t getDelta(std::time_t &time);
  bool isShown();
  void show();
  void hide();
  std::string &getName();
  std::string &getType();
  std::string toString();

private:
  Config &mConf;
  std::string mName, mNoradId, mSatelliteType;
  Tle mTLE;
  SGP4 mSGP4;
  CoordGeodetic mSatellitePosition;
  Trajectory mForwardTrajectory, mBackTrajectory;
  bool mShown;
  void calculate(std::tm &time);
  Tle loadTle();
};
