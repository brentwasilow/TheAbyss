#ifndef __SPRITE_H_INCLUDED__
#define __SPRITE_H_INCLUDED__

#include <SFML/Graphics.hpp>

class Sprite {
    public:
        double x, y;

        double distance;

        double angleWithOrigin;
        double playerViewingAngle;
        double angle;

        int xOffset;
        int yOffset;
        int texSize;

        char type;

        sf::Sprite sprite;
};

#endif
