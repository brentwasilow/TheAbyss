#include "player.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "constants.h"

Player::Player(const char* fileName) {
    std::string line;

    // open player configuration file
    std::ifstream file;
    file.open(fileName);

    // grab each line in the file and parse assigning values
    if (file.is_open()) {
        while (getline(file, line)) {
            std::string key = line.substr(0, line.find(Constants::DELIM));
            std::string token = line.substr(line.find(Constants::DELIM)+Constants::DELIM.length(), line.length());
            double value = atof(token.c_str());

            if (key.compare("x") == 0) {
                this->x = value;
            } else if (key.compare("y") == 0) {
                this->y = value;
            } else if (key.compare("movementSpeed") == 0) {
                this->movementSpeed = value;
            } else if (key.compare("angle") == 0) {
                this->angle = value;
            } else if (key.compare("angleSpeed") == 0) {
                this->angleSpeed = value;
            }
        }
    } else {
        std::cout << "Could not load Player file" << std::endl;
        exit(EXIT_FAILURE);
    }
    file.close();
}
