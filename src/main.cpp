#include <SFML/Graphics.hpp>
#include "render.h"
#include "player.h"
#include "update.h"
#include "constants.h"
#include "level.h"

void update(sf::RenderWindow&, Player&);
void render(sf::RenderWindow&, Player&);

int main(int argc, char* argv[]) {
    // create game variables
    sf::RenderWindow window;
    Render::initialize(window);

    // load player
    Player player (8*32, 8*32, 90);

    // load map
    Level level;
    level.initialize("res/level1.png");

    // define clock and ancillaries for game loop
    sf::Clock clock;
    sf::Int64 nextGameTick = clock.getElapsedTime().asMilliseconds();
    int loops;

    // run the program as long as the window is open
    while (window.isOpen()) {
        loops = 0;

        while (clock.getElapsedTime().asMilliseconds() > nextGameTick && loops < Constants::MAX_FRAMESKIP) {
            update(window, player);

            nextGameTick += Constants::SKIP_TICKS;
            loops++;
        }
        render(window, player);
    }
    return 0;
}

void update(sf::RenderWindow& window, Player& player) {
    Update::checkWindowState(window);

    switch(Update::state) {
        case 0:
            Update::checkTitleToGameState();
            break;
        case 1:
            Update::checkMovement(player);
            break;
    }
}

void render(sf::RenderWindow& window, Player& player) {
    window.clear(sf::Color::Black);

    switch(Update::state) {
        case 0:
            Render::drawTitleScreen(window);
            break;
        case 1:
            Render::drawBackground(window);
            Render::drawMap(window, player);
            break;
    }
    window.display();
}
