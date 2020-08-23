#include "Satellites.h"

#include "Config.h"
#include "Resources.h"
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
  auto res = Resources::getInstance();
  // TODO only do this if traj is recalcuklated or map zoomed
  res->iconBuffer.clear();
  res->trajectoryBuffer.clear();
  res->iconIndexBuf.clear();
  res->trajectoryIndexBuf.clear();

  for (auto &sat : UISats) { sat.render(zoom); }

  // render trajectories
  glBindBuffer(GL_ARRAY_BUFFER, res->trajectoryVBOId);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Resources::color_vertex) * res->trajectoryBuffer.size(),
               res->trajectoryBuffer.data(),
               GL_STREAM_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->trajectoryIBOId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(GLuint) * res->trajectoryIndexBuf.size(),
               res->trajectoryIndexBuf.data(),
               GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glUseProgram(res->trajectoryProgramId);

  // TODO save uniform location
  glUniform2f(glGetUniformLocation(res->textureProgramId, "zoom_center"), res->zcx, res->zcy);
  glUniform1f(glGetUniformLocation(res->textureProgramId, "zoom_level"), static_cast<float>(std::pow(2, zoom)));

  glBindVertexArray(res->trajectoryVAOId);
  glLineWidth(2.5f);
  glDrawElements(GL_LINE_STRIP, res->trajectoryIndexBuf.size(), GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
  glUseProgram(0);

  // render icons

  glBindBuffer(GL_ARRAY_BUFFER, res->iconVBOId);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Resources::texture_vertex) * res->iconBuffer.size(),
               res->iconBuffer.data(),
               GL_STREAM_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->iconIBOId);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * res->iconIndexBuf.size(), res->iconIndexBuf.data(), GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glUseProgram(res->textureProgramId);
  glBindTexture(GL_TEXTURE_2D, res->atlasTextureId);

  // TODO save uniform location
  glUniform1i(glGetUniformLocation(res->textureProgramId, "tex_sampl"), 0);
  glUniform2f(glGetUniformLocation(res->textureProgramId, "zoom_center"), res->zcx, res->zcy);
  glUniform1f(glGetUniformLocation(res->textureProgramId, "zoom_level"), static_cast<float>(std::pow(2, zoom)));

  glBindVertexArray(res->iconVAOId);
  glDrawElements(GL_TRIANGLES, res->iconIndexBuf.size(), GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
  glUseProgram(0);

  GL_CHECK;
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