#include "update.h"
#include "constants.h"
#include <cmath>
#include "wall.h"
#include "texture.h"
#include "weapon.h"
#include "enemy.h"
#include "sprite.h"
#include <iostream>
#define _USE_MATH_DEFINES

int Update::state = 1;
int Update::weaponOffset = 0;
double Update::timer = 0.0;
double Update::openDoorTimer = 0.0;
bool Update::openingDoor = false;
bool Update::closeDoor = false;
bool Update::waitingDoor = false;
int Update::doorRow = -1;
int Update::doorCol = -1;

void Update::checkWindowState(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void Update::checkIntroState() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
        state = 1;
    }
}

void Update::checkDoor(Player& player, Level& level) {
    // check player is standing in front of door and opening
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !openingDoor && !waitingDoor && !closeDoor) {
        int row = int(player.y)/Constants::TILE_SIZE;
        int col = int(player.x)/Constants::TILE_SIZE;
        if (level.map[row][col] == Wall::DOOR_TRIGGER) {
            if (row+1 < int(level.map.size()) && level.map[row+1][col] == Wall::DOOR) {
                doorRow = row+1;
                doorCol = col;
            } else if (row-1 >= 0 && level.map[row-1][col] == Wall::DOOR) {
                doorRow = row-1;
                doorCol = col;
            } else if (col-1 >= 0 && level.map[row][col-1] == Wall::DOOR) {
                doorRow = row;
                doorCol = col-1;
            } else if (col+1 < int(level.map[row].size()) && level.map[row][col+1] == Wall::DOOR) {
                doorRow = row;
                doorCol = col+1;
            }
            openingDoor = true;
        }
    }

    // opening door sequence
    if (openingDoor) {
        timer += 0.75;
        if (timer >= 64.0) {
            waitingDoor = true;
            openingDoor = false;
        }
    }

    // wait sequence
    if (waitingDoor) {
        openDoorTimer++;
        int row = int(player.y)/Constants::TILE_SIZE;
        int col = int(player.x)/Constants::TILE_SIZE;
        if (level.map[row][col] == Wall::DOOR) {
            return;
        }

        if (openDoorTimer >= Constants::DOOR_OPEN_TIME) {
            waitingDoor = false;
            openDoorTimer = 0.0;
            closeDoor = true;
        }
    }

    // closing door sequence
    if (closeDoor) {
        timer -= 0.75;
        if (timer <= 0.0) {
            closeDoor = false;
            timer = 0.0;
            doorRow = -1;
            doorCol = -1;
        }
    }
}

void Update::checkMovement(Player& player, Level& level) {
    // change viewing angle
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player.angle += player.angleSpeed;

        if (player.angle > 359.0) player.angle -= 360.0;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player.angle -= player.angleSpeed;

        if (player.angle < 0.0) player.angle += 360.0;
    }

    // change x and y coordinates
    double tempX = player.x;
    double tempY = player.y;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        player.x += player.movementSpeed * cos(player.angle * M_PI / 180.0);
        player.y += -player.movementSpeed * sin(player.angle * M_PI / 180.0);

        int row = int(player.y)/Constants::TILE_SIZE;
        int col = int(player.x)/Constants::TILE_SIZE;
        int block = level.map[row][col];

        if (block != Wall::EMPTY && block != Wall::DOOR_TRIGGER) {
            if (block == Wall::DOOR && row == doorRow && col == doorCol && waitingDoor) return;
            player.x = tempX;
            player.y = tempY;
        }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        player.x -= player.movementSpeed * cos(player.angle * M_PI / 180.0);
        player.y -= -player.movementSpeed * sin(player.angle * M_PI / 180.0);

        int row = int(player.y)/Constants::TILE_SIZE;
        int col = int(player.x)/Constants::TILE_SIZE;
        int block = level.map[row][col];

        if (block != Wall::EMPTY && block != Wall::DOOR_TRIGGER) {
            if (block == Wall::DOOR && row == doorRow && col == doorCol && waitingDoor) return;
            player.x = tempX;
            player.y = tempY;
        }
    }
}

void Update::checkWeapon(Player& player, Level& level) {
    // if attack key pressed initialize variables
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !player.attacking) {
        player.attacking = true;
        weaponOffset = 0;

        Weapon* fireball = new Weapon();
        fireball->setType('f');
        fireball->setPosition(int(player.x), int(player.y));
        fireball->setAngle(player.angle);

        level.sprites.push_back(fireball);
    }

    // while player is attacking lower rendering offset
    if (player.attacking) {
        weaponOffset += 2;
    }

    if (weaponOffset % 128 > 32) {
        player.attacking = false;
    }

    // if rendering offset is 0 stop animation
    if (weaponOffset == int(Texture::weapon.getSize().y)) {
        player.attacking = false;
    }
}

void Update::moveSprite(Player& player, Level& level) {
    for (uint i = 0; i < level.sprites.size(); i++) {
        if (level.sprites[i]->type == 'f') {
            Weapon* sprite = static_cast<Weapon*>(level.sprites[i]);

            sprite->x += 12 * cos(sprite->weaponAngle * M_PI / 180.0);
            sprite->y -= 12 * sin(sprite->weaponAngle * M_PI / 180.0);

            int block = level.map[int(sprite->y)/Constants::TILE_SIZE][int(sprite->x)/Constants::TILE_SIZE];
            if (block == !(Wall::EMPTY && Wall::DOOR)) {
                //delete sprite;
                level.sprites.erase(level.sprites.begin()+i);
            }

            for (uint j = 0; j < level.sprites.size(); j++) {
                if (level.sprites[j]->type == 'w') {
                     Enemy* enemy = static_cast<Enemy*>(level.sprites[j]);
                     if (int(enemy->x)/Constants::TILE_SIZE == int(sprite->x)/Constants::TILE_SIZE && int(enemy->y)/Constants::TILE_SIZE == int(sprite->y)/Constants::TILE_SIZE) {
                        if (!enemy->alive) {
                        } else {
                            enemy->health -= 25;
                            //delete sprite;
                            level.sprites.erase(level.sprites.begin()+i);
                        }
                    }
                }
            }
        }
    }
}

void Update::checkEnemies(Player& player, Level& level) {
    for (uint i = 0; i < level.sprites.size(); i++) {
        if (level.sprites[i]->type == 'f') {
            // do nothing
        } else if (level.sprites[i]->type == 'w') {
            Enemy* sprite = static_cast<Enemy*>(level.sprites[i]);
            // mark as dead
            if (sprite->health <= 0) {
                sprite->alive = false;
                sprite->moving = false;
                sprite->attacking = false;
                if (sprite->dyingAnimation < 59) {
                    sprite->dyingAnimation++;
                }
            } else {
                // spotted player so start moving
                if (sprite->distance < 256 && sprite->distance > 96) {
                    sprite->moving = true;
                    sprite->attacking = false;
                    sprite->attackingAnimation = 1;
                }

                // move toward player to close distance
                if (sprite->moving) {
                    double x = sprite->x - player.x;
                    double y = player.y - sprite->y;

                    x = x/sprite->distance;
                    y = y/sprite->distance;

                    sprite->x -= 2*x;
                    sprite->y += 2*y;
                }

                // close enough so start attacking
                if (sprite->distance <= 96) {
                    if (sprite->attacking == false) {
                        player.health -= 10;
                    }
                    sprite->attacking = true;
                    sprite->moving = false;
                }

                if (sprite->moving) {
                    sprite->movingAnimation++;
                }

                if (sprite->attacking) {
                    sprite->attackingAnimation++;
                    if (sprite->attackingAnimation % 120 == 0) player.health -= 10;
                }
            }
        }
    }
}

void Update::checkPlayer(Player& player) {
    if (player.health == 0) {
        Update::state = 2;
    }
}
