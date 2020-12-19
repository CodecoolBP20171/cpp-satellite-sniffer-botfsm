#include "Satellites.h"

#include "Config.h"
#include "LoadError.hpp"
#include "Resources.h"

#include <imgui.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

std::shared_ptr<Satellites> Satellites::instance = nullptr;

Satellites::Satellites() : mConf(Config::getInstance()) {
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

void Satellites::renderUISatellites(int zoom) {
  auto &res{Resources::getInstance()};
  // TODO only do this if trajectory is recalculated or map zoomed
  res.iconBuffer.clear();
  res.trajectoryBuffer.clear();
  res.iconIndexBuf.clear();
  res.trajectoryIndexBuf.clear();

  for (auto &sat : UISats) { sat.render(zoom); }

  renderTrajectories(zoom);
  renderIcons(zoom);

  GL_CHECK;
}
void Satellites::renderIcons(int zoom) const {
  auto &res{Resources::getInstance()};
  glBindBuffer(GL_ARRAY_BUFFER, res.iconVBOId);
  glBufferData(
      GL_ARRAY_BUFFER, sizeof(Graphics::texture_vertex) * res.iconBuffer.size(), res.iconBuffer.data(), GL_STREAM_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res.iconIBOId);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * res.iconIndexBuf.size(), res.iconIndexBuf.data(), GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glUseProgram(res.textureProgramId);
  glBindTexture(GL_TEXTURE_2D, res.atlasTextureId);

  // TODO move this somewhere else
  static auto icon_tsULoc{glGetUniformLocation(res.textureProgramId, "tex_sampl")};
  static auto icon_zcULoc{glGetUniformLocation(res.textureProgramId, "zoom_center")};
  static auto icon_zlULoc{glGetUniformLocation(res.textureProgramId, "zoom_level")};

  glUniform1i(icon_tsULoc, 0);
  glUniform2f(icon_zcULoc, res.zcx, res.zcy);
  glUniform1f(icon_zlULoc, static_cast<float>(std::pow(2, zoom)));

  glBindVertexArray(res.iconVAOId);
  glDrawElements(GL_TRIANGLES, res.iconIndexBuf.size(), GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
  glUseProgram(0);
}
void Satellites::renderTrajectories(int zoom) const {
  auto &res{Resources::getInstance()};
  glBindBuffer(GL_ARRAY_BUFFER, res.trajectoryVBOId);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Graphics::color_vertex) * res.trajectoryBuffer.size(),
               res.trajectoryBuffer.data(),
               GL_STREAM_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res.trajectoryIBOId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(GLuint) * res.trajectoryIndexBuf.size(),
               res.trajectoryIndexBuf.data(),
               GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glUseProgram(res.trajectoryProgramId);

  // TODO move this to some other place
  static auto line_zcULoc{glGetUniformLocation(res.trajectoryProgramId, "zoom_center")};
  static auto line_zlULoc{glGetUniformLocation(res.trajectoryProgramId, "zoom_level")};

  glUniform2f(line_zcULoc, res.zcx, res.zcy);
  glUniform1f(line_zlULoc, static_cast<float>(std::pow(2, zoom)));

  glBindVertexArray(res.trajectoryVAOId);
  glLineWidth(2.5f);
  glDrawElements(GL_LINE_STRIP, res.trajectoryIndexBuf.size(), GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
  glUseProgram(0);
}

void Satellites::renderPopupSatellites() {
  static auto height{sats.size() / 3 + 1};
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
  std::ifstream file(std::filesystem::path(mConf.getStringValue("/DataFiles/DATA_DIR").data()) /
                     mConf.getStringValue("/DataFiles/LIST_FILE").data());
  std::string line;
  while (std::getline(file, line)) {
    std::stringstream sat(line);
    std::string name, noradId, type;
    int visible;
    std::getline(sat, name, ';');
    std::getline(sat, noradId, ';');
    std::getline(sat, type, ';');
    sat >> visible;
    sats.emplace_back(Satellite{std::move(name), std::move(noradId), std::move(type), static_cast<bool>(visible)});
  }
  file.close();
}

void Satellites::saveSatelliteList() {
  std::ofstream file(std::filesystem::path(mConf.getStringValue("/DataFiles/DATA_DIR").data()) /
                         mConf.getStringValue("/DataFiles/LIST_FILE").data(),
                     std::ios_base::trunc);
  for (auto &sat : sats) { file << sat.toString(); }
  file.close();
}