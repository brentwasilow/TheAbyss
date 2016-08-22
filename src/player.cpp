#include "player.h"
#include "constants.h"

Player::Player() {
    // assign starting values to variables
    this->health = 100.0;
    this->attack = 1.0;
    this->attacking = false;
    this->xp = 0.0;
    this->height = 32.0;
    this->movementSpeed = 5.0;
    this->angle = 90.0;
    this->angleSpeed = 3.0;
}

void Player::setPosition(int x, int y) {
    this->x = (x * Constants::TILE_SIZE) + (Constants::TILE_SIZE_2);
    this->y = (y * Constants::TILE_SIZE) + (Constants::TILE_SIZE_2);
}
