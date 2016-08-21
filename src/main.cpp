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
            Update::checkIntroState();
            break;
        case 1:
            Update::checkMovement(player, level);
            Update::checkWeapon(player, level);
            Update::checkDoor(player, level);
            Update::checkPlayer(player);
            Update::checkEnemies(player, level);
            Update::moveSprite(player, level);
            break;
        case 2:
            window.close();
            break;
    }
}

void render(sf::RenderWindow& window, Player& player, Level& level) {
    window.clear(sf::Color::Black);

    switch(Update::state) {
        case 0:
            Render::drawIntroScreen(window);
            break;
        case 1:
            Render::drawMap(window, player, level);
            Render::drawSprites(window, player, level);
            Render::drawWeapon(window, player);
            break;
    }
    window.display();
}
