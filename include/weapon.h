#ifndef __WEAPON_H_INCLUDED__
#define __WEAPON_H_INCLUDED__

#include "sprite.h"

class Weapon: public Sprite {
    public:
        double weaponAngle;
        int ammo;

        Weapon();
        void setType(char);
        void setPosition(double, double);
        void setAngle(double);
};

#endif
