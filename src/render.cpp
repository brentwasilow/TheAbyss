#include "render.h"
#include "constants.h"
#include <cmath>
#include <iostream>

static sf::RectangleShape backgroundFloor;
static sf::RectangleShape backgroundCeiling;
static sf::Text titleText;
static sf::Font font;
static sf::Texture texture;
static sf::Sprite sprite;

double verticalIntersection(double, Player&);
double horizontalIntersection(double, Player&);

static int textureOffsetVertical;
static int textureOffsetHorizontal;
static int subimageOffsetVerticalX;
static int subimageOffsetVerticalY;
static int subimageOffsetHorizontalX;
static int subimageOffsetHorizontalY;

int map[8][8] = {{1, 1, 1, 1, 1, 1, 1, 1},
                 {1, 0, 0, 0, 0, 0, 0, 1},
                 {1, 0, 2, 0, 0, 2, 0, 1},
                 {1, 0, 0, 0, 0, 0, 0, 1},
                 {1, 0, 0, 0, 0, 0, 0, 1},
                 {1, 0, 2, 0, 0, 2, 0, 1},
                 {1, 0, 0, 0, 0, 0, 0, 1},
                 {1, 1, 1, 1, 1, 1, 1, 1}};

void Render::initialize(sf::RenderWindow& window) {
    // context settings for window
    sf::ContextSettings settings;
    settings.antialiasingLevel = Constants::ANTI_ALIASING_LEVEL;

    // create a window
    window.create(sf::VideoMode(Constants::WIDTH, Constants::HEIGHT), Constants::TITLE, sf::Style::Default, settings);

    // set window properties
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i((mode.width/2)-Constants::WIDTH_2, (mode.height/2)-(Constants::HEIGHT_2)));
    //window.setFramerateLimit(Constants::FPS);

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

    if (!texture.loadFromFile("res/textures.png")) {
        return;
    }
    sprite.setTexture(texture);
}

void Render::drawTitleScreen(sf::RenderWindow& window) {
    window.draw(titleText);
}

void Render::drawMap(sf::RenderWindow& window, Player& player) {
    double angle = player.angle + Constants::FOV_2_D;

    for (int x = 0; x < Constants::WIDTH; x++) {
        if (angle < 0.0) angle += 360.0;
        else if (angle >= 360.0) angle -= 360.0;

        double verticalDistance = verticalIntersection(angle, player);
        double horizontalDistance = horizontalIntersection(angle, player);

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
        int projectedSliceHeight = int((Constants::DISTANCE_TO_PROJECTION * Constants::TILE_SIZE) / correctDistance);

        sprite.setTextureRect(sf::IntRect(subimageOffsetX+textureOffset, subimageOffsetY, 1, 64));
        sf::Vector2f targetSize(1.0, projectedSliceHeight);
        sprite.setScale(targetSize.x/sprite.getLocalBounds().width, targetSize.y/sprite.getLocalBounds().height);
        sprite.setPosition(x, (Constants::HEIGHT_2)-(projectedSliceHeight/2));
        window.draw(sprite);

        angle -= Constants::ANGLE_BETWEEN_RAYS;
    }
    //exit(0);
}

double verticalIntersection(double angle, Player& player) {
    int verticalX;
    double  verticalY;
    int dx;
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
    int column = verticalX / Constants::TILE_SIZE;
    int row = int(verticalY) / Constants::TILE_SIZE;

    if (row < 0 || row >= 8 || column < 0 || column >= 8) return 100000000.0;

    while (map[row][column] == 0) {
        verticalX += dx;
        verticalY += dy;

        column = verticalX / Constants::TILE_SIZE;
        row = verticalY / Constants::TILE_SIZE;

        if (row < 0 || row >= 8 || column < 0 || column >= 8) return 100000000.0;
    }
    textureOffsetVertical = int(verticalY) - (row * Constants::TILE_SIZE);

    if (map[row][column] == 1) {
        subimageOffsetVerticalX = 0*65;
        subimageOffsetVerticalY = 2*65;
    } else if (map[row][column] == 2) {
        subimageOffsetVerticalX = 6*65;
        subimageOffsetVerticalY = 2*65;
    }
    double tempDistance = ((verticalX-player.x)*(verticalX-player.x))+
                          ((verticalY-player.y)*(verticalY-player.y));
    return sqrt(tempDistance);
}

double horizontalIntersection(double angle, Player& player) {
    double horizontalX;
    int horizontalY;
    double dx;
    int dy;
    double t = tan(angle * M_PI / 180.0);
    double it = tan(-angle * M_PI / 180.0);

    if (angle == 0.0 || angle == 180.0) return 100000000.0;

    if (angle > 0.0 && angle < 180.0) {
        horizontalY = int(player.y / Constants::TILE_SIZE) * Constants::TILE_SIZE;
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
        horizontalY = (int(player.y/Constants::TILE_SIZE)*Constants::TILE_SIZE) + Constants::TILE_SIZE;
        dy = Constants::TILE_SIZE;

        if (angle == -270.0) {
            horizontalX = player.x;
            dx = 0.0;
        } else {
            horizontalX = player.x - (player.y - horizontalY) / it;
            dx = Constants::TILE_SIZE / it;
        }
    }
    int column = int(horizontalX) / Constants::TILE_SIZE;
    int row = horizontalY / Constants::TILE_SIZE;

    if (row < 0 || row >= 8 || column < 0 || column >= 8) return 100000000.0;

    while (map[row][column] == 0) {
        horizontalX += dx;
        horizontalY += dy;

        column = int(horizontalX) / Constants::TILE_SIZE;
        row = horizontalY / Constants::TILE_SIZE;

        if (row < 0 || row >= 8 || column < 0 || column >= 8) return 100000000.0;
    }
    textureOffsetHorizontal = int(horizontalX) - (column * Constants::TILE_SIZE);

    if (map[row][column] == 1) {
        subimageOffsetHorizontalX = 0*65;
        subimageOffsetHorizontalY = 2*65;
    } else if (map[row][column] == 2) {
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
