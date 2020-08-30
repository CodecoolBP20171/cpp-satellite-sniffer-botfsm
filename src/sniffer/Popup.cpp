#include "Popup.h"

#include "Config.h"
#include "Globals.h"
#include "Resources.h"
#include "Satellites.h"

#include <imgui.h>

Popup::Popup(SDL_Rect rect, PState state, PState &programState) : UIElement(rect, state), state(programState) {}

bool Popup::isClicked(const SDL_MouseButtonEvent e, PState &state) { return false; }

void Popup::render() {
  ImGui::SetNextWindowPos(
      ImVec2(mConf.getIntValue("/Dimensions/POPUP_OFFSET_X"), mConf.getIntValue("/Dimensions/POPUP_OFFSET_Y")),
      ImGuiCond_Once);
  ImGui::SetNextWindowSize(
      ImVec2(mConf.getIntValue("/Dimensions/POPUP_WIDTH"), mConf.getIntValue("/Dimensions/POPUP_HEIGHT")),
      ImGuiCond_Once);
  if (ImGui::Begin("Choose which satellites to show.",
                   nullptr,
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
    ImGui::Columns(3, NULL, false);
    Satellites::getInstance()->renderPopupSatellits();
    ImGui::Columns(1);

    ImGui::Dummy(ImVec2(0, mConf.getIntValue("/Dimensions/MENU_BUTTON_SPACING")));
    ImGui::Dummy(ImVec2(0, 0));
    ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - (mConf.getIntValue("/Dimensions/MENU_BUTTON_WIDTH") / 2) -
                    mConf.getIntValue("/Dimensions/MENU_BUTTON_SPACING"));

    if (ImGui::Button("OK##popup_ok",
                      ImVec2(mConf.getIntValue("/Dimensions/MENU_BUTTON_WIDTH") / 2,
                             mConf.getIntValue("/Dimensions/MENU_BUTTON_HEIGHT") / 2))) {
      Satellites::getInstance()->saveSatelliteList();
      state = PState::RUNNING;
    }
    ImGui::End();
  }
}
