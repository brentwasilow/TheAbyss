#ifndef __ENEMY_H_INCLUDED__
#define __ENEMY_H_INCLUDED__

#include <SFML/Graphics.hpp>

class Enemy {
    public:
        double x, y;

        double distance;
        double angleWithOrigin;
        double playerViewingAngle;
        double angle;

        sf::Sprite sprite;
};

#endif
