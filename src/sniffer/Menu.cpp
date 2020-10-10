#include "Menu.h"

#include "Config.h"

#include <imgui.h>

Menu::Menu(SDL_Rect rect, PState state, PState &programState) : UIElement(rect, state), state(programState) {}

bool Menu::isClicked(const SDL_MouseButtonEvent &e, PState &state_) { return false; }

void Menu::render() {
  auto &menuDim = mConf.getRect("MENU");
  ImGui::SetNextWindowPos(ImVec2(static_cast<float>(menuDim.x), static_cast<float>(menuDim.y)), ImGuiCond_Once);
  ImGui::SetNextWindowSize(ImVec2(static_cast<float>(menuDim.w), static_cast<float>(menuDim.h)), ImGuiCond_Once);
  ImGui::SetNextWindowBgAlpha(0.f);
  if (ImGui::Begin("menu##menubar",
                   nullptr,
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | // NOLINT(hicpp-signed-bitwise)
                       ImGuiWindowFlags_NoResize |                         // NOLINT(hicpp-signed-bitwise)
                       ImGuiWindowFlags_NoCollapse)) {                     // NOLINT(hicpp-signed-bitwise)
    if (ImGui::Button("SATELLITES##menu_sats",
                      ImVec2(static_cast<float>(mConf.getIntValue("/Dimensions/MENU_BUTTON_WIDTH")),
                             static_cast<float>(mConf.getIntValue("/Dimensions/MENU_BUTTON_HEIGHT"))))) {
      state = PState::PAUSED;
    }
    ImGui::SameLine(ImGui::GetWindowContentRegionMax().x -
                    static_cast<float>(mConf.getIntValue("/Dimensions/MENU_BUTTON_WIDTH") -
                                       mConf.getIntValue("/Dimensions/MENU_BUTTON_SPACING")));

    if (ImGui::Button("QUIT##menu_quit",
                      ImVec2(static_cast<float>(mConf.getIntValue("/Dimensions/MENU_BUTTON_WIDTH")),
                             static_cast<float>(mConf.getIntValue("/Dimensions/MENU_BUTTON_HEIGHT"))))) {
      state = PState::QUIT;
    }
    ImGui::End();
  }
}
