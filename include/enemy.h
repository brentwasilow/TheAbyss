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
        double fangle;
        bool moving;
        bool alive;
        bool attacking;
        int health;
        int movingAnimation;
        int attackingAnimation;
        int dyingAnimation;
        int xOffset;
        int yOffset;
        int texSize;
        char type;

        sf::Sprite sprite;
};

#endif
