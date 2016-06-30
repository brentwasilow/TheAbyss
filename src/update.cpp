#include "update.h"
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

void Update::checkMovement(Player& player) {
    int row = 0, column = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player.angle -= 3.0;

        if (player.angle < 0.0) player.angle += 360.0;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player.angle += 3.0;

        if (player.angle > 359.0) player.angle -= 360.0;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        player.x += player.speed * cos(player.angle * M_PI / 180.0);
        player.y += -player.speed * sin(player.angle * M_PI / 180.0);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        player.x -= player.speed * cos(player.angle * M_PI / 180.0);
        player.y -= -player.speed * sin(player.angle * M_PI / 180.0);
    }
}
