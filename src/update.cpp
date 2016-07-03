#include "update.h"
#include "constants.h"
#include <cmath>

#define _USE_MATH_DEFINES

int Update::state = 0;

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

void Update::checkMovement(Player& player, Level& level) {
    // change viewing angle
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player.angle -= player.angleSpeed;

        if (player.angle < 0.0) player.angle += 360.0;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player.angle += player.angleSpeed;

        if (player.angle > 359.0) player.angle -= 360.0;
    }

    // change x and y coordinates
    double tempX = player.x;
    double tempY = player.y;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        player.x += player.movementSpeed * cos(player.angle * M_PI / 180.0);
        player.y += -player.movementSpeed * sin(player.angle * M_PI / 180.0);

        if (level.map[int(player.y)/Constants::TILE_SIZE][int(player.x)/Constants::TILE_SIZE]!=Constants::EMPTY) {
            player.x = tempX;
            player.y = tempY;
        }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        player.x -= player.movementSpeed * cos(player.angle * M_PI / 180.0);
        player.y -= -player.movementSpeed * sin(player.angle * M_PI / 180.0);

        if (level.map[int(player.y)/Constants::TILE_SIZE][int(player.x)/Constants::TILE_SIZE]!=Constants::EMPTY) {
            player.x = tempX;
            player.y = tempY;
        }
    }
}
