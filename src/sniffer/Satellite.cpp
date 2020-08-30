#include "Satellite.h"

#include "Config.h"
#include "Globals.h"
#include "Resources.h"

#include <CoordGeodetic.h>
#include <SGP4.h>

#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define _CRT_SECURE_NO_WARNINGS

Satellite::Satellite(std::string name, std::string noradId, std::string type, bool visible)
    : mName(name), mNoradId(noradId), mTLE(loadTle()), mSatelliteType(type), mSGP4(mTLE), mShown(visible),
      mForwardTrajectory(Trajectory::FORWARD), mBackTrajectory(Trajectory::BACK), mConf(Config::getInstance()) {
  updatePosition();
}

Tle Satellite::loadTle() {
  std::ifstream file(
      (std::filesystem::path(mConf.getStringValue("/DataFiles/DATA_DIR")) / mNoradId).replace_extension("dat"));
  std::string tle1, tle2;
  std::getline(file, tle1);
  std::getline(file, tle2);
  return Tle(mName, tle1, tle2);
}

void Satellite::calculate(std::tm &time) {
  DateTime tm(time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
  Eci eci(mSGP4.FindPosition(tm));
  mSatellitePosition = eci.ToGeodetic();
}

CoordGeodetic Satellite::getPositionAtTime(std::time_t &time) {
  auto *stime = std::gmtime(&time);
  DateTime tm(stime->tm_year + 1900, stime->tm_mon + 1, stime->tm_mday, stime->tm_hour, stime->tm_min, stime->tm_sec);
  Eci eci(mSGP4.FindPosition(tm));
  CoordGeodetic geo(eci.ToGeodetic());
  geo.longitude += MathConstants::PI;
  geo.latitude -= MathConstants::PI / 2;
  geo.latitude = -geo.latitude;
  return geo;
}

CoordGeodetic &Satellite::getPosition() { return mSatellitePosition; }

Trajectory &Satellite::getForwardTrajectory() { return mForwardTrajectory; }

Trajectory &Satellite::getBackTrajectory() { return mBackTrajectory; }

void Satellite::transformOrigo() {
  mSatellitePosition.longitude += MathConstants::PI;
  mSatellitePosition.latitude -= MathConstants::PI / 2;
  mSatellitePosition.latitude = -mSatellitePosition.latitude;
}

void Satellite::updatePosition() {
  std::time_t time;
  std::time(&time);
  updatePosition(time);
}

void Satellite::updatePosition(std::time_t time) {
  if (!mShown) return;
  auto *tmptime = gmtime(&time);
  std::tm stime;
  std::memcpy(&stime, tmptime, sizeof(std::tm));
  calculate(stime);
  mForwardTrajectory.calculate(time, *this);
  mBackTrajectory.calculate(time, *this);
  transformOrigo();
}

Satellite::~Satellite() {}

void Satellite::toggleShown() {
  mShown = !mShown;
  if (mShown) updatePosition();
}

time_t Satellite::getDelta(std::time_t &time) {
  OrbitalElements oe(mTLE);
  auto pos(getPositionAtTime(time));
  auto rate(pos.altitude / oe.RecoveredSemiMajorAxis());
  return static_cast<time_t>(round(rate / 24));
}

bool Satellite::isShown() { return mShown; }

void Satellite::show() {
  if (!mShown) {
    mShown = true;
    updatePosition();
  }
}

void Satellite::hide() { mShown = false; }

std::string &Satellite::getName() { return mName; }

std::string &Satellite::getType() { return mSatelliteType; }

std::string Satellite::toString() {
  std::stringstream line;
  line << mName << ';' << mNoradId << ';' << mSatelliteType << ';' << (mShown ? '1' : '0') << '\n';
  return line.str();
}
