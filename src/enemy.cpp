#include "enemy.h"
#include "texture.h"
#include "constants.h"

Enemy::Enemy() {
    this->alive = true;
    this->health = 100;

    this->moving = false;
    this->attacking = false;

    this->movingAnimation = 0;
    this->attackingAnimation = 0;
    this->dyingAnimation = 0;

    this->sprite.setTexture(Texture::sprites);
}

void Enemy::setType(char type) {
    this->type = type;

    switch (type) {
        case 'w':
            this->xOffset = 0;
            this->yOffset = 256;
            this->texSize = 64;
            break;
    }
}

void Enemy::setPosition(int x, int y) {
    this->x = (x * Constants::TILE_SIZE) + (Constants::TILE_SIZE/2);
    this->y = (y * Constants::TILE_SIZE) + (Constants::TILE_SIZE/2);
}

