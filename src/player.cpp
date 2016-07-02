#include "player.h"

Player::Player(double x, double y, double angle) {
    this->x = x;
    this->y = y;
    this->movementSpeed = 5.0;

    this->angle = angle;
    this->angleSpeed = 3.0;
}
