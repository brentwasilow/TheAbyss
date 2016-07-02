#include "level.h"
#include <SFML/Graphics.hpp>
#include <stdlib.h>

std::vector<std::vector<int> > Level::loadMap(const char* fileName) {
    // load level image
    sf::Image level;
    if (!level.loadFromFile(fileName)) {
        exit(EXIT_FAILURE);
    }
    // create 2D array
    std::vector<std::vector<int> > map;
    map.resize(level.getSize().y, std::vector<int>(level.getSize().x, 0));

    // populate map from image loaded
    for (uint y = 0; y < level.getSize().y; y++) {
        for (uint x = 0; x < level.getSize().x; x++) {
            sf::Color color = level.getPixel(x, y);

            if (color == sf::Color::White) {
                map[y][x] = Constants::EMPTY;
            } else if (color == sf::Color::Black) {
                map[y][x] = Constants::BRICK;
            } else if (color == sf::Color::Blue) {
                map[y][x] = Constants::FLAG;
            } else if (color == sf::Color::Red) {
                player.x = (x * Constants::TILE_SIZE) + Constants::TILE_SIZE/2;
                player.y = (y * Constants::TILE_SIZE) + Constants::TILE_SIZE/2;
                map[y][x] = Constants::EMPTY;
            }
        }
    }
    return map;
}
