#include "stdafx.h"
#include "Menu.h"
#include "Config.h"
#include <imgui.h>

Menu::Menu(SDL_Rect rect, PState state) : UIElement(rect, state)
{}

bool Menu::isClicked(const int x, const int y, PState & state)
{
	if (UIElement::isClicked(x, y, state)) {
		for (auto& button : menuButtons) {
			if (button->isClicked(x, y, state)) {
				break;
			}
		}
		return true;
	}
	return false;
}

void Menu::render(PState* state)
{
	auto& menuDim = Config::getRect("MENU");
	ImGui::SetNextWindowPos(ImVec2(menuDim.x, menuDim.y), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(menuDim.w, menuDim.h), ImGuiCond_Once);
	ImGui::SetNextWindowBgAlpha(0.0f);
	if (ImGui::Begin("menu##menubar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
		if (ImGui::Button("SATELLITES##menu_sats", ImVec2(Config::getIntOption("Dimensions", "MENU_BUTTON_WIDTH"), Config::getIntOption("Dimensions", "MENU_BUTTON_HEIGHT")))) {
			*state = PState::PAUSED;
		}
		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - (Config::getIntOption("Dimensions", "MENU_BUTTON_WIDTH")) - Config::getIntOption("Dimensions", "MENU_BUTTON_SPACING"));

		if (ImGui::Button("QUIT##menu_quit", ImVec2(Config::getIntOption("Dimensions", "MENU_BUTTON_WIDTH"), Config::getIntOption("Dimensions", "MENU_BUTTON_HEIGHT")))) {
			*state = PState::QUIT;
		}
		ImGui::End();
	}
}

