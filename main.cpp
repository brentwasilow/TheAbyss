#include <iostream>
#include <SFML/Graphics.hpp>

#define WIDTH 640
#define HEIGHT 480
#define TITLE "The Abyss"
#define ANTI_ALIASING_LEVEL 8
#define FPS 60

int main(int argc, char* argv[]) {
    // context settings for window
    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTI_ALIASING_LEVEL;

    // create a window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), TITLE, sf::Style::Default, settings);

    // set window properties
    window.setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width/2)-(WIDTH/2), (sf::VideoMode::getDesktopMode().height/2)-(HEIGHT/2)));
    window.setFramerateLimit(FPS);

    // create ceiling and floor background shapes/colors/positions
    sf::RectangleShape floor(sf::Vector2f(WIDTH, HEIGHT/2));
    floor.setFillColor(sf::Color(50, 50, 50));
    floor.setPosition(0, 0);

    sf::RectangleShape ceiling(sf::Vector2f(WIDTH, HEIGHT/2));
    ceiling.setFillColor(sf::Color(100, 100, 100));
    floor.setPosition(0, HEIGHT/2);


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
        window.draw(floor);
        window.draw(ceiling);

        // end the current frame
        window.display();
    }
    return 0;
}
