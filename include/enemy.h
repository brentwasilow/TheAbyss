#ifndef __ENEMY_H_INCLUDED__
#define __ENEMY_H_INCLUDED__

#include "sprite.h"

class Enemy: public Sprite {
    public:
        bool alive;
        int health;

        bool moving;
        bool attacking;


        int movingAnimation;
        int attackingAnimation;
        int dyingAnimation;

        Enemy();
        void setType(char);
        void setPosition(int, int);
};

#endif
