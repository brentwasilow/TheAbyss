#include <SFML/Graphics.hpp>
#include "render.h"
#include "player.h"
#include "update.h"
#include "constants.h"
#include "level.h"
#include "texture.h"
#include <iostream>
#include <ctime>

void update(sf::RenderWindow&, Player&, Level&);
void render(sf::RenderWindow&, Player&, Level&);

int main(int argc, char* argv[]) {
    // create and initialize game variables
    sf::RenderWindow window;
    Render::setup(window);
    Texture::initialize();
    Player player("res/level1.plyr");
    Level level("res/level1.png", player);

    // define clock and ancillaries for game loop
    sf::Clock clock;
    sf::Int64 nextGameTick = clock.getElapsedTime().asMilliseconds();
    int loops;

    // run the program as long as the window is open
    while (window.isOpen()) {
        loops = 0;

        while (clock.getElapsedTime().asMilliseconds() > nextGameTick && loops < Constants::MAX_FRAMESKIP) {
            update(window, player, level);

            nextGameTick += Constants::SKIP_TICKS;
            loops++;
        }
        render(window, player, level);
    }
    Render::cleanup();
    return 0;
}

void update(sf::RenderWindow& window, Player& player, Level& level) {
    Update::checkWindowState(window);

    switch(Update::state) {
        case 0:
            Update::checkTitleToGameState();
            break;
        case 1:
            Update::checkMovement(player, level);
            Update::checkWeapon(player);
            break;
    }
}

void render(sf::RenderWindow& window, Player& player, Level& level) {
    window.clear(sf::Color::Black);

    switch(Update::state) {
        case 0:
            Render::drawTitleScreen(window);
            break;
        case 1:
            Render::drawMap(window, player, level);
//            Render::drawEnemies(window, player, level);
//            Render::drawWeapon(window, player);
            break;
    }
    window.display();
}
