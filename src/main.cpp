#include <SFML/Graphics.hpp>
#include "render.h"
#include "player.h"

static int state = 0;

int main(int argc, char* argv[]) {
    // create and initialize window
    sf::RenderWindow window;
    Render::initialize(window);

    // define player for use in rendering and updating
    Player player;
    player.angle = 90.0;
    player.x = 160.0;
    player.y = 160.0;

    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events
        sf::Event event;
        while (window.pollEvent(event)) {
            // event - close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        // clear the window with black color
        window.clear(sf::Color::Black);

        // determine game state
        switch(state) {
            case 0: // Intro
                // render title screen
                Render::drawTitleScreen(window);

                // if return key change to next game state
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                    state = 1;
                }
                break;
            case 1: // Main Game
                // render background
                Render::drawBackground(window);
                Render::drawMap(window, player);
                break;
        }
        // end the current frame
        window.display();
    }
    return 0;
}
