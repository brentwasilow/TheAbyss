#include "level.h"

#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include "enemy.h"
#include "texture.h"
#include "constants.h"
#include <iostream>
Level::Level(const char* fileName, Player& player) {
    // load level image
    sf::Image level;
    if (!level.loadFromFile(fileName)) {
        exit(EXIT_FAILURE);
    }
    // create 2D array
    map.resize(level.getSize().y, std::vector<int>(level.getSize().x, 0));

    // assign size of zBuffer
    zBuffer.resize(Constants::WIDTH);

    // initialize size of enemies list
    enemies.resize(0);

    // populate map from image loaded
    for (uint y = 0; y < level.getSize().y; y++) {
        for (uint x = 0; x < level.getSize().x; x++) {
            sf::Color color = level.getPixel(x, y);

            if (color == sf::Color::White) {
                map[y][x] = Constants::EMPTY;
            } else if (color == sf::Color::Black) {
                map[y][x] = Constants::BRICK;
            } else if (color == sf::Color::Blue) {
                map[y][x] = Constants::DOOR;
            } else if (color == sf::Color::Red) {
                player.x = (x * Constants::TILE_SIZE)+Constants::TILE_SIZE/2;
                player.y = (y * Constants::TILE_SIZE)+Constants::TILE_SIZE/2;
                map[y][x] = Constants::EMPTY;
            } else if (color == sf::Color::Magenta) {
                map[y][x] = Constants::DOOR_TRIGGER;
            } else if (color == sf::Color::Yellow) {
                Enemy enemy;
                enemy.x = (x * Constants::TILE_SIZE)+Constants::TILE_SIZE/2;
                enemy.y = (y * Constants::TILE_SIZE)+Constants::TILE_SIZE/2;
                enemy.moving = false;
                enemy.alive = true;
                enemy.fangle = 0;
                enemy.attacking = false;
                enemy.health = 100;
                enemy.movingAnimation = 0;
                enemy.attackingAnimation = 0;
                enemy.dyingAnimation = 0;
                enemy.xOffset = 0;
                enemy.yOffset = 256;
                enemy.texSize = 64;
                enemy.type = 'e';

                enemy.sprite.setTexture(Texture::sprites);
                enemies.push_back(enemy);
            } else {
                map[y][x] = Constants::EMPTY;
            }
        }
    }
}
