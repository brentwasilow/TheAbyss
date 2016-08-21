#ifndef __CONSTANTS_H_INCLUDED__
#define __CONSTANTS_H_INCLUDED__

#include <string>

namespace Constants {
    extern const int WIDTH;
    extern const int WIDTH_2;
    extern const int HEIGHT;
    extern const int HEIGHT_2;

    extern const char* TITLE;
    extern const int ANTI_ALIASING_LEVEL;

    extern const double FPS;
    extern const double UPS;

    extern const double FOV_D;
    extern const double FOV_R;
    extern const double FOV_2_D;
    extern const double FOV_2_R;

    extern const int DISTANCE_TO_PROJECTION;
    extern const double ANGLE_BETWEEN_RAYS;
    extern const int TILE_SIZE;
    extern const int TILE_SIZE_2;
    extern const int DISTANCE_TO_PROJECTION_TILE_SIZE;

    extern const int TICKS_PER_SECOND;
    extern const int SKIP_TICKS;
    extern const int MAX_FRAMESKIP;

    extern const double DOOR_OPEN_TIME;

    extern const std::string DELIM;

    extern const int PIXEL_BUFFER_SIZE;
};

#endif
