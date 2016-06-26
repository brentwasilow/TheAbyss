#include <SFML/Graphics.hpp>
#include "render.h"

int main(int argc, char* argv[]) {
    // create and initialize window
    sf::RenderWindow window;
    Render::init(window);

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

        // draw everything here...
        Render::drawBackground(window);

        // end the current frame
        window.display();
    }
    return 0;
}
