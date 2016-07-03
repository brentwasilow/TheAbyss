#include <SFML/Graphics.hpp>
#include "render.h"
#include "player.h"
#include "update.h"
#include "constants.h"
#include "level.h"
#include "texture.h"
#include <iostream>

void update(sf::RenderWindow&, Player&, Level&);
void render(sf::RenderWindow&, Player&, Level&);

int main(int argc, char* argv[]) {
    // create window
    sf::RenderWindow window;
    Render::initialize(window);

    // load player
    Player player("res/level1.plyr");

    // load level
    Level level("res/level1.png", player);

    // load textures
    Texture::initialize();

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
            Render::drawBackground(window);
            Render::drawMap(window, player, level);
            break;
    }
    window.display();
}
