#include "constants.h"
#include <cmath>

#define _USE_MATH_DEFINES

namespace Constants {
    const int WIDTH = 640;
    const int WIDTH_2 = (WIDTH/2);
    const int HEIGHT = 480;
    const int HEIGHT_2 = (HEIGHT/2);

    const char* TITLE = "The Abyss";
    const int ANTI_ALIASING_LEVEL = 8;

    const double FPS = 60.0;
    const double UPS = 60.0;

    const double FOV_D = 60.0;
    const double FOV_R = FOV_D * (M_PI/180.0);
    const double FOV_2_D = (FOV_D/2);
    const double FOV_2_R = FOV_2_D * (M_PI/180.0);

    const int DISTANCE_TO_PROJECTION = (WIDTH_2/tan(FOV_2_R));
    const double ANGLE_BETWEEN_RAYS = (FOV_D/WIDTH);
    const int TILE_SIZE = 64;
    const int TILE_SIZE_2 = TILE_SIZE/2;
    const int DISTANCE_TO_PROJECTION_TILE_SIZE = DISTANCE_TO_PROJECTION * TILE_SIZE;

    const int TICKS_PER_SECOND = 60;
    const int SKIP_TICKS = (1000/TICKS_PER_SECOND);
    const int MAX_FRAMESKIP = 10;

    const double DOOR_OPEN_TIME = 300.0;

    const std::string DELIM = ":=";

    const int PIXEL_BUFFER_SIZE = (WIDTH * HEIGHT * 4);
};
