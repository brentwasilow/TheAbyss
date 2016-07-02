#include <SFML/Graphics.hpp>
#include "render.h"
#include "player.h"
#include "update.h"
#include "constants.h"

void update(sf::RenderWindow&, Player&);
void render(sf::RenderWindow&, Player&);

int main(int argc, char* argv[]) {
    // create and initialize window
    sf::RenderWindow window;
    Render::initialize(window);

    // define player for use in rendering and updating
    Player player;
    player.x = 8*32;
    player.y = 8*32;
    player.angle = 90.0;
    player.speed = 5.0;

    // load level
    sf::Image level;
    if (!level.loadFromFile("res/level1.png")) {
        return -1;
    }

    int map[level.getSize().y][level.getSize().x];
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
            } else {

            }
        }
    }

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
