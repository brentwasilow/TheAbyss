#ifndef __ENEMY_H_INCLUDED__
#define __ENEMY_H_INCLUDED__

#include "sprite.h"

class Enemy: public Sprite {
    public:
//        double x, y;

//        double distance;
//        double angleWithOrigin;
//        double playerViewingAngle;
//        double angle;
//        double fangle;
        bool alive;
        int health;

        bool moving;
        bool attacking;

        int movingAnimation;
        int attackingAnimation;
        int dyingAnimation;
//        int xOffset;
//        int yOffset;
//        int texSize;
//        char type;

//        sf::Sprite sprite;

        Enemy();
        void setType(char);
        void setPosition(int, int);
};

#endif
