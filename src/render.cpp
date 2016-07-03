#include "render.h"
#include "constants.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include "texture.h"

static sf::RectangleShape backgroundFloor;
static sf::RectangleShape backgroundCeiling;

double verticalIntersection(double, Player&, Level&);
double horizontalIntersection(double, Player&, Level&);

static int textureOffsetVertical;
static int textureOffsetHorizontal;
static int subimageOffsetVerticalX;
static int subimageOffsetVerticalY;
static int subimageOffsetHorizontalX;
static int subimageOffsetHorizontalY;

void Render::initialize(sf::RenderWindow& window) {
    // context settings
    sf::ContextSettings settings;
    settings.antialiasingLevel = Constants::ANTI_ALIASING_LEVEL;

    // create the window
    window.create(sf::VideoMode(Constants::WIDTH, Constants::HEIGHT), Constants::TITLE, sf::Style::Default, settings);

    // set window properties
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i((mode.width/2)-Constants::WIDTH_2, (mode.height/2)-(Constants::HEIGHT_2)));

    // create floor and ceiling background sizes/colors/positions
    backgroundFloor.setSize(sf::Vector2f(Constants::WIDTH, Constants::HEIGHT_2));
    backgroundFloor.setFillColor(sf::Color(50, 50, 50));
    backgroundFloor.setPosition(0, Constants::HEIGHT_2);

    backgroundCeiling.setSize(sf::Vector2f(Constants::WIDTH, Constants::HEIGHT_2));
    backgroundCeiling.setFillColor(sf::Color(100, 100, 100));
    backgroundCeiling.setPosition(0, 0);
}

void Render::drawTitleScreen(sf::RenderWindow& window) {
    window.draw(Texture::titleText);
}

void Render::drawMap(sf::RenderWindow& window, Player& player, Level& level) {
    double angle = player.angle + Constants::FOV_2_D;

    for (int x = 0; x <= Constants::WIDTH; x++) {
        if (angle < 0.0) angle += 360.0;
        else if (angle >= 360.0) angle -= 360.0;

        double verticalDistance = verticalIntersection(angle, player, level);
        double horizontalDistance = horizontalIntersection(angle, player, level);

        double distance;
        int textureOffset;
        int subimageOffsetX;
        int subimageOffsetY;

        if (horizontalDistance < verticalDistance) {
            distance = horizontalDistance;
            textureOffset = textureOffsetHorizontal;
            subimageOffsetX = subimageOffsetHorizontalX;
            subimageOffsetY = subimageOffsetHorizontalY;
        } else {
            distance = verticalDistance;
            textureOffset = textureOffsetVertical;
            subimageOffsetX = subimageOffsetVerticalX;
            subimageOffsetY = subimageOffsetVerticalY;
        }
        double correctDistance = distance * cos((angle - player.angle) * M_PI / 180.0);
        double projectedSliceHeight = Constants::DISTANCE_TO_PROJECTION * Constants::TILE_SIZE / correctDistance;

        Texture::wallTextureSprite.setTextureRect(sf::IntRect(subimageOffsetX+textureOffset, subimageOffsetY, 1, 64));
        sf::Vector2f targetSize(1.0f, projectedSliceHeight);
        Texture::wallTextureSprite.setScale(targetSize.x/Texture::wallTextureSprite.getLocalBounds().width, targetSize.y/Texture::wallTextureSprite.getLocalBounds().height);
        Texture::wallTextureSprite.setPosition(x, (Constants::HEIGHT_2)-(projectedSliceHeight/2.0));
        window.draw(Texture::wallTextureSprite);

        angle -= Constants::ANGLE_BETWEEN_RAYS;
    }
}

double verticalIntersection(double angle, Player& player, Level& level) {
    double verticalX;
    double verticalY;
    double dx;
    double dy;
    double t = tan(angle * M_PI / 180.0);

    if (angle == 90.0 || angle == 270.0) return 100000000.0;

    if (angle > 90.0 && angle < 270.0) {
        verticalX = (int(player.x) / Constants::TILE_SIZE) * Constants::TILE_SIZE;
        dx = -Constants::TILE_SIZE;
        verticalY = player.y + (player.x - verticalX) * t;
        dy = t * Constants::TILE_SIZE;
        verticalX--;
    } else {
        verticalX = ((int(player.x) / Constants::TILE_SIZE) * Constants::TILE_SIZE) + Constants::TILE_SIZE;
        dx = Constants::TILE_SIZE;
        verticalY = player.y + (player.x - verticalX) * t;
        dy = -t * Constants::TILE_SIZE;
    }
    int column = int(verticalX) / Constants::TILE_SIZE;
    int row = int(verticalY) / Constants::TILE_SIZE;

    if (row < 0 || row >= int(level.map.size()) || column < 0 || column >= int(level.map[0].size())) return 100000000.0;

    while (level.map[row][column] == 0) {
        verticalX += dx;
        verticalY += dy;

        column = int(verticalX) / Constants::TILE_SIZE;
        row = int(verticalY) / Constants::TILE_SIZE;

        if (row < 0 || row >= int(level.map.size()) || column < 0 || column >= int(level.map[0].size())) return 100000000.0;
    }
    textureOffsetVertical = int(verticalY) - (row * Constants::TILE_SIZE);

    if (level.map[row][column] == 1) {
        subimageOffsetVerticalX = 0*65;
        subimageOffsetVerticalY = 2*65;
    } else if (level.map[row][column] == 2) {
        subimageOffsetVerticalX = 6*65;
        subimageOffsetVerticalY = 2*65;
    }
    double tempDistance = ((verticalX-player.x)*(verticalX-player.x))+
                          ((verticalY-player.y)*(verticalY-player.y));
    return sqrt(tempDistance);
}

double horizontalIntersection(double angle, Player& player, Level& level) {
    double horizontalX;
    double horizontalY;
    double dx;
    double dy;
    double t = tan(angle * M_PI / 180.0);
    double it = tan(-angle * M_PI / 180.0);

    if (angle == 0.0 || angle == 180.0) return 100000000.0;

    if (angle > 0.0 && angle < 180.0) {
        horizontalY = (int(player.y) / Constants::TILE_SIZE) * Constants::TILE_SIZE;
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
        horizontalY = ((int(player.y)/Constants::TILE_SIZE)*Constants::TILE_SIZE) + Constants::TILE_SIZE;
        dy = Constants::TILE_SIZE;

        if (angle == -270.0) {
            horizontalX = player.x;
            dx = 0.0;
        } else {
            horizontalX = player.x - ((player.y - horizontalY) / it);
            dx = Constants::TILE_SIZE / it;
        }
    }
    int column = int(horizontalX) / Constants::TILE_SIZE;
    int row = int(horizontalY) / Constants::TILE_SIZE;

    if (row < 0 || row >= int(level.map.size()) || column < 0 || column >= int(level.map[0].size())) return 100000000.0;

    while (level.map[row][column] == 0) {
        horizontalX += dx;
        horizontalY += dy;

        column = int(horizontalX) / Constants::TILE_SIZE;
        row = int(horizontalY) / Constants::TILE_SIZE;

        if (row < 0 || row >= int(level.map.size()) || column < 0 || column >= int(level.map[0].size())) return 100000000.0;
    }
    textureOffsetHorizontal = int(horizontalX) - (column * Constants::TILE_SIZE);

    if (level.map[row][column] == 1) {
        subimageOffsetHorizontalX = 0*65;
        subimageOffsetHorizontalY = 2*65;
    } else if (level.map[row][column] == 2) {
        subimageOffsetHorizontalX = 6*65;
        subimageOffsetHorizontalY = 2*65;
    }
    double tempDistance = (horizontalX-player.x)*(horizontalX-player.x)+
                          (horizontalY-player.y)*(horizontalY-player.y);
    return sqrt(tempDistance);
}

void Render::drawBackground(sf::RenderWindow& window) {
    window.draw(backgroundFloor);
    window.draw(backgroundCeiling);
}
