#include "render.h"
#include "constants.h"
#include <cmath>
#include <iostream>
#define _USE_MATH_DEFINES

static sf::RectangleShape backgroundFloor;
static sf::RectangleShape backgroundCeiling;
static sf::Text titleText;
static sf::Font font;

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
    backgroundFloor.setSize(sf::Vector2f(Constants::WIDTH, Constants::HEIGHT/2));
    backgroundFloor.setFillColor(sf::Color(50, 50, 50));
    backgroundFloor.setPosition(0, Constants::HEIGHT/2);

    backgroundCeiling.setSize(sf::Vector2f(Constants::WIDTH, Constants::HEIGHT/2));
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
    titleText.setPosition(sf::Vector2f(Constants::WIDTH/2.0f, Constants::HEIGHT/2.0f));
}

void Render::drawTitleScreen(sf::RenderWindow& window) {
    window.draw(titleText);
}

int map[5][5] = {{1, 1, 1, 1, 1},
                 {1, 0, 0, 0, 1},
                 {1, 0, 0, 0, 1},
                 {1, 0, 0, 0, 1},
                 {1, 1, 1, 1, 1}};

double viewingAngle = 90.0;
double angleBetweenRays = Constants::FOV / (double)(Constants::WIDTH);
int tileSize = 64.0;

void Render::drawMap(sf::RenderWindow& window) {
    double fovRadians = ((Constants::FOV/2.0)*M_PI/180.0);
    double distanceToProjection = Constants::WIDTH/ 2.0 / tan(fovRadians);
    double angle = viewingAngle + (Constants::FOV/2.0);

    for (int x = 0; x < Constants::WIDTH; x++) {
        if (angle < 0.0) angle += 360.0;
        else if (angle >= 360.0) angle -= 360.0;

        double verticalDistance = Render::verticalIntersection(angle);
        double horizontalDistance = Render::horizontalIntersection(angle);

        double distance;
        if (horizontalDistance < verticalDistance) {
            distance = horizontalDistance;
        } else {
            distance = verticalDistance;
        }
        double correctDistance = distance * cos((angle - viewingAngle) * M_PI / 180.0);
        double projectedSliceHeight = distanceToProjection * tileSize / correctDistance;

        sf::RectangleShape wall;
        wall.setSize(sf::Vector2f(1.0, projectedSliceHeight));
        wall.setFillColor(sf::Color(255, 0, 255));
        wall.setPosition(x, (Constants::HEIGHT/2.0)-(projectedSliceHeight/2.0));

        window.draw(wall);

        angle -= angleBetweenRays;
    }
}

double x = 160.0;
double y = 160.0;

double Render::verticalIntersection(double angle) {
    double verticalX, verticalY;
    double dx, dy;
    double t = tan(angle * M_PI / 180.0);

    if (angle == 90.0 || angle == 270.0) return 1000000.0;

    if (angle > 90.0 && angle < 270.0) {
        verticalX = (x / tileSize) * tileSize;
        dx = -tileSize;
        verticalY = y + ((x - verticalX) * t);
        dy = t * tileSize;
        verticalX--;
    } else {
        verticalX = ((x / tileSize) * tileSize) + tileSize;
        dx = tileSize;
        verticalY = y + ((x - verticalX) * t);
        dy = -t * tileSize;
    }
    int column = (int) (verticalX / tileSize);
    int row = (int) (verticalY / tileSize);

    if (row < 0 || row >= 5 || column < 0 || column >= 5) return 1000000.0;

    while (map[row][column] == 0) {
        verticalX += dx;
        verticalY += dy;

        column = (int) (verticalX / tileSize);
        row = (int) (verticalY / tileSize);

        if (row < 0 || row >= 5 || column < 0 || column >= 5) return 1000000.0;
    }
    double tempDistance = ((verticalX-x)*(verticalX-x))+((verticalY-y)*(verticalY-y));
    return sqrt(tempDistance);
}

double Render::horizontalIntersection(double angle) {
    double horizontalX, horizontalY;
    double dx, dy;
    double t = tan(angle * M_PI / 180.0);
    double it = tan(-angle * M_PI / 180.0);

    if (angle == 0.0 || angle == 180.0) return 1000000.0;

    if (angle > 0.0 && angle < 180.0) {
        horizontalY = (y / tileSize) * tileSize;
        dy = -tileSize;

        if (angle == 90.0) {
            horizontalX = x;
            dx = 0.0;
        } else {
            horizontalX = x + ((y - horizontalY) / t);
            dx = tileSize / t;
        }
        horizontalY--;
    } else {
        horizontalY = ((y/tileSize)*tileSize) + tileSize;
        dy = tileSize;

        if (angle == 270.0) {
            horizontalX = x;
            dx = 0.0;
        } else {
            horizontalX = x - ((y - horizontalY) / it);
            dx = tileSize / it;
        }
    }
    int column = (int) (horizontalX / tileSize);
    int row = (int) (horizontalY / tileSize);

    if (row < 0 || row >= 5 || column < 0 || column >= 5) return 1000000.0;

    while (map[row][column] == 0) {
        horizontalX += dx;
        horizontalY += dy;

        column = (int) (horizontalX / tileSize);
        row = (int) (horizontalY / tileSize);

        if (row < 0 || row >= 5 || column < 0 || column >= 5) return 1000000.0;
    }
    double tempDistance = ((horizontalX-x)*(horizontalX-x))+((horizontalY-y)*(horizontalY-y));
    return sqrt(tempDistance);
}

void Render::drawBackground(sf::RenderWindow& window) {
    window.draw(backgroundFloor);
    window.draw(backgroundCeiling);
}
