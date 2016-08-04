#include "update.h"
#include "constants.h"
#include <cmath>
#include "texture.h"
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

void Update::checkTitleToGameState() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
        state = 1;
    }
}

void Update::checkDoor(Player& player, Level& level) {
    // check player is standing in front of door and opening
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !openingDoor && !waitingDoor && !closeDoor) {
        int row = int(player.y)/Constants::TILE_SIZE;
        int col = int(player.x)/Constants::TILE_SIZE;
        if (level.map[row][col] == Constants::DOOR_TRIGGER) {
            if (row+1 < int(level.map.size()) && level.map[row+1][col] == Constants::DOOR) {
                doorRow = row+1;
                doorCol = col;
            } else if (row-1 >= 0 && level.map[row-1][col] == Constants::DOOR) {
                doorRow = row-1;
                doorCol = col;
            } else if (col-1 >= 0 && level.map[row][col-1] == Constants::DOOR) {
                doorRow = row;
                doorCol = col-1;
            } else if (col+1 < int(level.map[row].size()) && level.map[row][col+1] == Constants::DOOR) {
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
        if (openDoorTimer == Constants::DOOR_OPEN_TIME) {
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

        if (block != Constants::EMPTY && block != Constants::DOOR_TRIGGER) {
            if (block == Constants::DOOR && row == doorRow && col == doorCol && waitingDoor) return;
            player.x = tempX;
            player.y = tempY;
        }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        player.x -= player.movementSpeed * cos(player.angle * M_PI / 180.0);
        player.y -= -player.movementSpeed * sin(player.angle * M_PI / 180.0);

        int row = int(player.y)/Constants::TILE_SIZE;
        int col = int(player.x)/Constants::TILE_SIZE;
        int block = level.map[row][col];

        if (block != Constants::EMPTY && block != Constants::DOOR_TRIGGER) {
            if (block == Constants::DOOR && row == doorRow && col == doorCol && waitingDoor) return;
            player.x = tempX;
            player.y = tempY;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        player.offset = 128;
    } else {
        player.offset = 0;
    }
}

void Update::checkWeapon(Player& player) {
    // if attack key pressed initialize variables
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        player.attacking = true;
        weaponOffset = 0;
    }

    // while player is attacking lower rendering offset
    if (player.attacking) {
        weaponOffset += 2;
    }

    // if rendering offset is 0 stop animation
    if (weaponOffset == int(Texture::weapon.getSize().y)) {
        player.attacking = false;
    }
}
