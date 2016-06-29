#include "render.h"
#include "constants.h"
#include <cmath>

static sf::RectangleShape backgroundFloor;
static sf::RectangleShape backgroundCeiling;
static sf::Text titleText;
static sf::Font font;

double verticalIntersection(double, Player&);
double horizontalIntersection(double, Player&);

int map[5][5] = {{1, 1, 1, 1, 1},
                 {1, 0, 0, 0, 1},
                 {1, 0, 0, 0, 1},
                 {1, 0, 0, 0, 1},
                 {1, 1, 1, 1, 1}};

void Render::initialize(sf::RenderWindow& window) {
    // context settings for window
    sf::ContextSettings settings;
    settings.antialiasingLevel = Constants::ANTI_ALIASING_LEVEL;

    // create a window
    window.create(sf::VideoMode(Constants::WIDTH, Constants::HEIGHT), Constants::TITLE, sf::Style::Default, settings);

    // set window properties
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i((mode.width/2)-Constants::WIDTH_2, (mode.height/2)-(Constants::HEIGHT_2)));
    window.setFramerateLimit(Constants::FPS);

    // create floor and ceiling background sizes/colors/positions
    backgroundFloor.setSize(sf::Vector2f(Constants::WIDTH, Constants::HEIGHT_2));
    backgroundFloor.setFillColor(sf::Color(50, 50, 50));
    backgroundFloor.setPosition(0, Constants::HEIGHT_2);

    backgroundCeiling.setSize(sf::Vector2f(Constants::WIDTH, Constants::HEIGHT_2));
    backgroundCeiling.setFillColor(sf::Color(100, 100, 100));
    backgroundCeiling.setPosition(0, 0);

    // create title font/text
    if (!font.loadFromFile("res/TitleFont.ttf")) {
        return;
    }
    titleText.setFont(font);
    titleText.setString("The Abyss");
    titleText.setCharacterSize(50);
    titleText.setColor(sf::Color::White);
    sf::FloatRect textRect = titleText.getLocalBounds();
    titleText.setOrigin(textRect.left + textRect.width/2.0f,
                        textRect.top  + textRect.height/2.0f);
    titleText.setPosition(sf::Vector2f(Constants::WIDTH_2, Constants::HEIGHT_2));
}

void Render::drawTitleScreen(sf::RenderWindow& window) {
    window.draw(titleText);
}

void Render::drawMap(sf::RenderWindow& window, Player& player) {
    double distanceToProjection = Constants::WIDTH_2 / tan(Constants::FOV_2_R);
    double angle = player.angle + Constants::FOV_2_D;

    for (int x = 0; x < Constants::WIDTH; x++) {
        if (angle < 0.0) angle += 360.0;
        else if (angle >= 360.0) angle -= 360.0;

        double verticalDistance = verticalIntersection(angle, player);
        double horizontalDistance = horizontalIntersection(angle, player);

        double distance;
        if (horizontalDistance < verticalDistance) {
            distance = horizontalDistance;
        } else {
            distance = verticalDistance;
        }
        double correctDistance = distance * cos((angle - player.angle) * M_PI / 180.0);
        double projectedSliceHeight = distanceToProjection * Constants::TILE_SIZE / correctDistance;

        sf::RectangleShape wall;
        wall.setSize(sf::Vector2f(1.0, projectedSliceHeight));
        wall.setFillColor(sf::Color(255, 0, 255));
        wall.setPosition(x, (Constants::HEIGHT_2)-(projectedSliceHeight/2.0));

        window.draw(wall);

        angle -= Constants::ANGLE_BETWEEN_RAYS;
    }
}

double verticalIntersection(double angle, Player& player) {
    double verticalX, verticalY;
    double dx, dy;
    double t = tan(angle * M_PI / 180.0);

    if (angle == 90.0 || angle == 270.0) return 1000000.0;

    if (angle > 90.0 && angle < 270.0) {
        verticalX = (player.x / Constants::TILE_SIZE) * Constants::TILE_SIZE;
        dx = -Constants::TILE_SIZE;
        verticalY = player.y + ((player.x - verticalX) * t);
        dy = t * Constants::TILE_SIZE;
        verticalX--;
    } else {
        verticalX = ((player.x / Constants::TILE_SIZE) * Constants::TILE_SIZE) + Constants::TILE_SIZE;
        dx = Constants::TILE_SIZE;
        verticalY = player.y + ((player.x - verticalX) * t);
        dy = -t * Constants::TILE_SIZE;
    }
    int column = (int) (verticalX / Constants::TILE_SIZE);
    int row = (int) (verticalY / Constants::TILE_SIZE);

    if (row < 0 || row >= 5 || column < 0 || column >= 5) return 1000000.0;

    while (map[row][column] == 0) {
        verticalX += dx;
        verticalY += dy;

        column = (int) (verticalX / Constants::TILE_SIZE);
        row = (int) (verticalY / Constants::TILE_SIZE);

        if (row < 0 || row >= 5 || column < 0 || column >= 5) return 1000000.0;
    }
    double tempDistance = ((verticalX-player.x)*(verticalX-player.x))+
                          ((verticalY-player.y)*(verticalY-player.y));
    return sqrt(tempDistance);
}

double horizontalIntersection(double angle, Player& player) {
    double horizontalX, horizontalY;
    double dx, dy;
    double t = tan(angle * M_PI / 180.0);
    double it = tan(-angle * M_PI / 180.0);

    if (angle == 0.0 || angle == 180.0) return 1000000.0;

    if (angle > 0.0 && angle < 180.0) {
        horizontalY = (player.y / Constants::TILE_SIZE) * Constants::TILE_SIZE;
        dy = -Constants::TILE_SIZE;

        if (angle == 90.0) {
            horizontalX = player.x;
            dx = 0.0;
        } else {
            horizontalX = player.x + ((player.y - horizontalY) / t);
            dx = Constants::TILE_SIZE / t;
        }
        horizontalY--;
    } else {
        horizontalY = ((player.y/Constants::TILE_SIZE)*Constants::TILE_SIZE) + Constants::TILE_SIZE;
        dy = Constants::TILE_SIZE;

        if (angle == 270.0) {
            horizontalX = player.x;
            dx = 0.0;
        } else {
            horizontalX = player.x - ((player.y - horizontalY) / it);
            dx = Constants::TILE_SIZE / it;
        }
    }
    int column = (int) (horizontalX / Constants::TILE_SIZE);
    int row = (int) (horizontalY / Constants::TILE_SIZE);

    if (row < 0 || row >= 5 || column < 0 || column >= 5) return 1000000.0;

    while (map[row][column] == 0) {
        horizontalX += dx;
        horizontalY += dy;

        column = (int) (horizontalX / Constants::TILE_SIZE);
        row = (int) (horizontalY / Constants::TILE_SIZE);

        if (row < 0 || row >= 5 || column < 0 || column >= 5) return 1000000.0;
    }
    double tempDistance = ((horizontalX-player.x)*(horizontalX-player.x))+
                          ((horizontalY-player.y)*(horizontalY-player.y));
    return sqrt(tempDistance);
}

void Render::drawBackground(sf::RenderWindow& window) {
    window.draw(backgroundFloor);
    window.draw(backgroundCeiling);
}
