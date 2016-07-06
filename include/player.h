#ifndef __PLAYER_H_INCLUDED__
#define __PLAYER_H_INCLUDED__

class Player {
    public:
        double health;
        double xp;
        double attack;

        double angle;
        double angleSpeed;

        double x;
        double y;
        double movementSpeed;

        Player(const char*);
};

#endif
