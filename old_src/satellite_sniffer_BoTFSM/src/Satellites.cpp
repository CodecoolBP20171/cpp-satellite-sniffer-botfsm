#include "Satellites.h"

#include "Config.h"
#include "stdafx.h"

#include <LoadError.hpp>

#include <imgui.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::shared_ptr<Satellites> Satellites::instance = nullptr;

Satellites::Satellites() {
  loadSatellites();
  for (auto &sat : sats) { UISats.emplace_back(sat); }
}

std::shared_ptr<Satellites> &Satellites::getInstance() {
  if (!instance) { instance = std::shared_ptr<Satellites>(new Satellites()); }
  return instance;
}

void Satellites::updatePosition() {
  for (auto &sat : sats) { sat.updatePosition(); }
}

void Satellites::renderUISatellits(int zoom) {
  for (auto &sat : UISats) { sat.render(zoom); }
}

void Satellites::renderPopupSatellits() {
  static int height = sats.size() / 3 + 1;
  int cnt = 0;
  for (auto &sat : sats) {
    if (ImGui::Selectable(sat.getName().c_str(), sat.isShown())) { sat.toggleShown(); }
    cnt++;
    if (cnt == height) {
      cnt = 0;
      ImGui::NextColumn();
    }
  }
  ImGui::NextColumn();
}

void Satellites::loadSatellites() {
  std::ifstream file(Config::getStringOption("DataFiles", "DATA_DIR") + "/" +
                     Config::getStringOption("DataFiles", "LIST_FILE"));
  std::string line;
  while (std::getline(file, line)) {
    std::stringstream sat(line);
    std::string name, noradId, type;
    int visible;
    std::getline(sat, name, ';');
    std::getline(sat, noradId, ';');
    std::getline(sat, type, ';');
    sat >> visible;
    sats.push_back(Satellite{name, noradId, type, static_cast<bool>(visible)});
  }
  file.close();
}

void Satellites::saveSatelliteList() {
  std::ofstream file(Config::getStringOption("DataFiles", "DATA_DIR") + "/" +
                         Config::getStringOption("DataFiles", "LIST_FILE"),
                     std::ios_base::trunc);
  for (auto &sat : sats) { file << sat.toString(); }
  file.close();
}