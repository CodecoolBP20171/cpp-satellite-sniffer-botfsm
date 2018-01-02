#pragma once
#include <string>

namespace Dimensions {
	/* pixel dimensions */
	const int MAP_WIDTH = 1024;
	const int MAP_HEIGHT = 512;
	const int MENU_HEIGHT = 100;
	const int MAP_TEXT_SIZE = 64;
	const int WINDOW_WIDTH = MAP_WIDTH;
	const int WINDOW_HEIGHT = MAP_HEIGHT + MENU_HEIGHT;
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