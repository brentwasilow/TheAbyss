#include "level.h"

#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include "enemy.h"
#include "constants.h"
#include "texture.h"
#include "wall.h"
#include <iostream>
Level::Level(const char* fileName, Player& player) {
    // load level image
    sf::Image level;
    if (!level.loadFromFile(fileName)) {
        exit(EXIT_FAILURE);
    }
    // create 2D array of map
    map.resize(level.getSize().y, std::vector<int>(level.getSize().x, 0));

    // assign size of zBuffer
    zBuffer.resize(Constants::WIDTH);

    // initialize size of enemies list
    sprites.resize(0);

    // populate map from image loaded
    for (uint y = 0; y < level.getSize().y; y++) {
        for (uint x = 0; x < level.getSize().x; x++) {
            sf::Color color = level.getPixel(x, y);

            if (color == sf::Color::White) {
                map[y][x] = Wall::EMPTY;
            } else if (color == sf::Color::Black) {
                map[y][x] = Wall::RED_BRICK;
            } else if (color == sf::Color::Blue) {
                map[y][x] = Wall::DOOR;
            } else if (color == sf::Color::Magenta) {
                map[y][x] = Wall::DOOR_TRIGGER;
            } else if (color == sf::Color::Red) {
                // set player position in the center of the tile and make block empty
                player.setPosition(x, y);
                map[y][x] = Wall::EMPTY;
            } else if (color == sf::Color::Yellow) {
                // create wizard and add to sprite list
                Enemy* wizard = new Enemy();

                wizard->setType('w');
                wizard->setPosition(x, y);

                sprites.push_back(wizard);
            }
        }
    }
}
