#include "level.h"
#include <SFML/Graphics.hpp>
#include <stdlib.h>

std::vector<vector<int>> Level::loadMap(const char* fileName) {
    // load level image
    sf::Image level;
    if (!level.loadFromFile(fileName)) {
        exit(EXIT_FAILURE);
    }

    std::vector<vector<int>> map;
}
