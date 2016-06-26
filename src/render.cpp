#include "render.h"
#include "constants.h"

static sf::RectangleShape floor;
static sf::RectangleShape ceiling;

void Render::init(sf::RenderWindow& window) {
    // context settings for window
    sf::ContextSettings settings;
    settings.antialiasingLevel = Constants::ANTI_ALIASING_LEVEL;

    // create a window
    window.create(sf::VideoMode(Constants::WIDTH, Constants::HEIGHT), Constants::TITLE, sf::Style::Default, settings);

    // set window properties
    window.setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width/2)-(Constants::WIDTH/2), (sf::VideoMode::getDesktopMode().height/2)-(Constants::HEIGHT/2)));
    window.setFramerateLimit(Constants::FPS);

    // create floor and ceiling background sizes/colors/positions
    floor.setSize(sf::Vector2f(Constants::WIDTH, Constants::HEIGHT/2));
    floor.setFillColor(sf::Color(50, 50, 50));
    floor.setPosition(0, Constants::HEIGHT/2);

    ceiling.setSize(sf::Vector2f(Constants::WIDTH, Constants::HEIGHT/2));
    ceiling.setFillColor(sf::Color(100, 100, 100));
    ceiling.setPosition(0, 0);
}

void Render::drawBackground(sf::RenderWindow& window) {
    window.draw(floor);
    window.draw(ceiling);
}
