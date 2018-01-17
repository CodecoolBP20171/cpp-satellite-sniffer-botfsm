#pragma once
#include <string>
#include <SDL.h>

namespace MathConstants {
	const double PI = acos(-1);
}

namespace Dimensions {
	/* pixel dimensions */
	const int MAP_WIDTH = 1024;
	const int MAP_HEIGHT = 512;
	const int MENU_HEIGHT = 100;
	const int MAP_TEXT_SIZE = 64;
	const int WINDOW_WIDTH = MAP_WIDTH;
	const int WINDOW_HEIGHT = MAP_HEIGHT + MENU_HEIGHT;
	const int MENU_BUTTON_WIDTH = 200;
	const int MENU_BUTTON_HEIGHT = 70;
	const int MENU_BUTTON_SPACING = 15;
	const int MENU_BUTTON_VERTICAL_SPACING = (MENU_HEIGHT - MENU_BUTTON_HEIGHT) / 2;
	const int POPUP_WIDTH = WINDOW_WIDTH - MENU_BUTTON_WIDTH;
	const int POPUP_HEIGHT = MAP_HEIGHT;
	const int POPUP_OFFSET_X = (WINDOW_WIDTH - POPUP_WIDTH) / 2;
	const int POPUP_OFFSET_Y = (WINDOW_HEIGHT - POPUP_HEIGHT) / 2;
}

namespace FontFiles {
	const std::string MAP_TEXT = "monofonto.ttf";
}

namespace TextureFiles {
	const std::string MAP = "map_s.png";
	const std::string PATH = "trajectory_s.png";
	const std::string STATION = "station_s.png";
	const std::string TELESCOPE = "telescope_s.png";
	const std::string GPS = "gps_s.png";
	const std::string SAT_BUTTON = "satellite_button_active_s.png";
	const std::string OK_BUTTON = "ok_button_active_s.png";
}

namespace SatelliteType {
	const std::string STATION = "STATION";
	const std::string TELESCOPE = "TELESCOPE";
	const std::string GPS = "GPS";
}

namespace ButtonName {
	const std::string SATELLITES = "SATELLITES";
	const std::string OK = "OK";
}

namespace UIRects {
	const SDL_Rect MENU{ 0, 0, Dimensions::WINDOW_WIDTH, Dimensions::MENU_HEIGHT };
	const SDL_Rect POPUP{ Dimensions::POPUP_OFFSET_X,
						Dimensions::POPUP_OFFSET_Y,
						Dimensions::POPUP_WIDTH,
						Dimensions::POPUP_HEIGHT };
	const SDL_Rect MAP{ 0,
						Dimensions::MENU_HEIGHT,
						Dimensions::MAP_WIDTH,
						Dimensions::MAP_HEIGHT };
}

namespace TrajectoryLimits {
	const int MAX_POINTS = 100;
	const double MAX_DISTANCE = 0.4;
	const double DISTANCE_GAP = 0.05;
	const int RENDER_DISTANCE_GAP = 100;
}

namespace TrajectoryRender {
	const Uint8 LINE_WEIGHT = 8;
	/* SDL2_gfx is fcked we need ABGR: 0xAABBGGRR */
	const Uint32 FORWARD_COLOR = 0xDD19FCF4;
	const Uint32 BACKWARD_COLOR = 0xDD0A0066;
}