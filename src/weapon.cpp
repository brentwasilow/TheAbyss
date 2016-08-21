#include "weapon.h"
#include "texture.h"

Weapon::Weapon() {
    this->sprite.setTexture(Texture::sprites);
}

void Weapon::setType(char type) {
    this->type = type;

    switch (type) {
        case 'f':
            this->xOffset = 256;
            this->yOffset = 0;
            this->texSize = 64;
            this->ammo = 25;
    }
}

void Weapon::setPosition(double x, double y) {
    this->x = x;
    this->y = y;
}

void Weapon::setAngle(double angle) {
    this->weaponAngle = angle;
}
