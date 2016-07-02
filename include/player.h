#ifndef __PLAYER_H_INCLUDED__
#define __PLAYER_H_INCLUDED__

class Player {
    public:
        double angle;
        double angleSpeed;

        double x;
        double y;
        double movementSpeed;

        Player(double, double, double);
};

#endif
