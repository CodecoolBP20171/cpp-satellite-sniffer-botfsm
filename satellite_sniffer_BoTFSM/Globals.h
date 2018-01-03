#pragma once
#include <string>

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
	const int MENU_BUTTON_VERTICAL_SPACING = (MENU_HEIGHT + MENU_BUTTON_HEIGHT) / 2;
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
}

namespace SatelliteType {
	const std::string STATION = "STATION";
	const std::string TELESCOPE = "TELESCOPE";
	const std::string GPS = "GPS";
}

namespace ButtonName {
	const std::string SATELITES = "SATELITES";
}