#ifndef __PLAYER_H_INCLUDED__
#define __PLAYER_H_INCLUDED__

class Player {
    public:
        double x, y;

        double health;

        double attack;
        bool attacking;

        double xp;

        double angle;
        double angleSpeed;
        double movementSpeed;

        double height;

        Player();
        void setPosition(int, int);
};

#endif
