#include "render.h"
#include "constants.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include "texture.h"
#include "update.h"

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

static const double fogValue = (16*Constants::TILE_SIZE);
static const sf::Vector2f scale(1.0f, 1.0f);
static const float numerator = 32.0f * Constants::DISTANCE_TO_PROJECTION;
static const int floorTexOffsetX = 1*65;
static const int floorTexOffsetY = 0*65;

void Render::initialize(sf::RenderWindow& window) {
    // context settings
    sf::ContextSettings settings;
    settings.antialiasingLevel = Constants::ANTI_ALIASING_LEVEL;

    // create the window
    //window.create(sf::VideoMode(Constants::WIDTH, Constants::HEIGHT), Constants::TITLE, sf::Style::Default, settings);

    window.create(sf::VideoMode(Constants::WIDTH, Constants::HEIGHT), "The Abyss");

    window.setVerticalSyncEnabled(false);

    // set window properties
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i((mode.width/2)-Constants::WIDTH_2, (mode.height/2)-(Constants::HEIGHT_2)));

    // create floor and ceiling background sizes/colors/positions
    backgroundFloor.setSize(sf::Vector2f(Constants::WIDTH, Constants::HEIGHT_2));
    backgroundFloor.setFillColor(sf::Color(0, 0, 0));
    backgroundFloor.setPosition(0, Constants::HEIGHT_2);

    backgroundCeiling.setSize(sf::Vector2f(Constants::WIDTH, Constants::HEIGHT_2));
    backgroundCeiling.setFillColor(sf::Color(0, 0, 0));
    backgroundCeiling.setPosition(0, 0);
}

void Render::drawTitleScreen(sf::RenderWindow& window) {
    window.draw(Texture::titleText);
}

void Render::drawMap(sf::RenderWindow& window, Player& player, Level& level) {
    double angle = player.angle + Constants::FOV_2_D;

    sf::Uint8 pixels[Constants::WIDTH * Constants::HEIGHT * 4];
    sf::Texture floorAndCeilingTexture;
    floorAndCeilingTexture.create(Constants::WIDTH, Constants::HEIGHT);
    sf::Sprite floorAndCeilingSprite(floorAndCeilingTexture);
    std::memset(pixels, 0, sizeof(pixels));

    for (int x = 0; x < Constants::WIDTH; x++) {
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
        double correctDistance = distance*cos((angle-player.angle)*M_PI/180.0);
        double projectedSliceHeight = Constants::DISTANCE_TO_PROJECTION*Constants::TILE_SIZE/correctDistance;

        level.zBuffer[x] = distance;

        // depth shading
        double wallDepth = distance / fogValue;
        if (wallDepth > 1.0) wallDepth = 1.0;
        wallDepth *= 255;
        int wallCol = 255-wallDepth;
        sf::Color wallDepthShade(wallCol, wallCol, wallCol);

        double wallScale = projectedSliceHeight / Constants::TILE_SIZE;

        int wallCounter = 0;

//        Texture::wallTextureSprite.setColor(wallDepthShade);

        for (int y = (Constants::HEIGHT_2-int(projectedSliceHeight/2)); y < (Constants::HEIGHT_2+int(projectedSliceHeight/2)); y++) {
            if (y < 0 || y >= Constants::HEIGHT) {
                wallCounter++;
                continue;
            }
            int index = (y * Constants::WIDTH + x) * 4;
            sf::Color wallColor = Texture::wallTextureImage.getPixel(subimageOffsetX+textureOffset, subimageOffsetY+int(wallCounter/wallScale));
            wallColor *= wallDepthShade;

            pixels[index] = wallColor.r;
            pixels[index+1] = wallColor.g;
            pixels[index+2] = wallColor.b;
            pixels[index+3] = wallColor.a;

            wallCounter++;
        }

//        Texture::wallTextureSprite.setTextureRect(sf::IntRect(subimageOffsetX+textureOffset, subimageOffsetY, 1, 64));
//        sf::Vector2f targetSize(1.0f, projectedSliceHeight);
//        Texture::wallTextureSprite.setScale(targetSize.x/Texture::wallTextureSprite.getLocalBounds().width, targetSize.y/Texture::wallTextureSprite.getLocalBounds().height);
//        Texture::wallTextureSprite.setPosition(x, (Constants::HEIGHT_2)-(projectedSliceHeight/2.0));
//        window.draw(Texture::wallTextureSprite);

        int counter = 0;
//        Texture::wallTextureSprite.setScale(scale.x, scale.y);

        float correction = 1/cos((player.angle-angle)*M_PI/180);
        float xComponent = cos(angle*M_PI/180);
        float yComponent = -sin(angle*M_PI/180);
        int denom = int(projectedSliceHeight/2);

        int ceilingOffset = Constants::HEIGHT_2-int(projectedSliceHeight/2.0f)-1;

        for (int y = Constants::HEIGHT_2+int(projectedSliceHeight/2); y < Constants::HEIGHT; y++) {
            float wallDistance = numerator / denom;
            float correctWallDistance = wallDistance * correction;
            denom++;

            float changeInX = (xComponent * correctWallDistance) + player.x;
            float changeInY = (yComponent * correctWallDistance) + player.y;

            int texX = int(changeInX) & 63;
            int texY = int(changeInY) & 63;

            // depth shading
            double floorDepth = correctWallDistance / fogValue;
            if (floorDepth > 1.0) floorDepth = 1.0;
            floorDepth *= 255;
            int floorCol = 255-floorDepth;
            sf::Color floorDepthShade(floorCol, floorCol, floorCol);

            sf::Color color = Texture::wallTextureImage.getPixel(floorTexOffsetX+texX, floorTexOffsetY+texY);
            color *= floorDepthShade;

            int floorIndex = (y * Constants::WIDTH + x) * 4;
            int ceilingIndex = ((ceilingOffset-counter) * Constants::WIDTH + x) * 4;

            pixels[floorIndex] = color.r;
            pixels[floorIndex + 1] = color.g;
            pixels[floorIndex + 2] = color.b;
            pixels[floorIndex + 3] = color.a;

            pixels[ceilingIndex] = color.r;
            pixels[ceilingIndex + 1] = color.g;
            pixels[ceilingIndex + 2] = color.b;
            pixels[ceilingIndex + 3] = color.a;

            //Texture::wallTextureSprite.setTextureRect(sf::IntRect((floorTexOffsetX)+texX, (floorTexOffsetY)+texY, 1, 1));
            //Texture::wallTextureSprite.setPosition(x, y);
            //window.draw(Texture::wallTextureSprite);

            //Texture::wallTextureSprite.setScale(1.0f, -1.0f);
            //Texture::wallTextureSprite.setPosition(x, Constants::HEIGHT_2-1-int(projectedSliceHeight/2.0f)-counter);
            //window.draw(Texture::wallTextureSprite);

            counter++;
        }
        angle -= Constants::ANGLE_BETWEEN_RAYS;
    }
    floorAndCeilingTexture.update(pixels);
    window.draw(floorAndCeilingSprite);
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

void Render::drawEnemies(sf::RenderWindow& window, Player& player, Level& level) {
    // loop through enemy list
    for (uint i = 0; i < level.enemies.size(); i++) {
        // player angle in radians
        double playerAngleRadians = (player.angle * M_PI / 180.0);

        // get change in x and y for player angle
        double vx = cos(playerAngleRadians);
        double vy = sin(playerAngleRadians);

        // get change in x/y from enemy to player
        double dx = level.enemies[i].x - player.x;
        double dy = player.y - level.enemies[i].y;

        // get straight line distance from enemy to player
        level.enemies[i].distance = hypot(dx, dy);

        // find angle difference between enemy and player
        double dPrimeX = dx / level.enemies[i].distance;
        level.enemies[i].angleWithOrigin = acos(dPrimeX) * 180.0 / M_PI;

        // enemy is in front of player so change angle to III and IV quadrant
        if (dy < 0) level.enemies[i].angleWithOrigin = 360 - level.enemies[i].angleWithOrigin;

        level.enemies[i].playerViewingAngle = player.angle;

        // get angle difference between enemy and player factoring in position in cartesian quadrants
        if ((dy < 0 && dx > 0) && (vx > 0 && vy >= 0)) {
            level.enemies[i].playerViewingAngle = player.angle + 360;
        } else if ((dy > 0 && dx > 0) && (vx > 0 && vy < 0)) {
            level.enemies[i].angleWithOrigin += 360;
        }
        level.enemies[i].angle = level.enemies[i].playerViewingAngle - level.enemies[i].angleWithOrigin;
    }


    // sort enemy list as a z-buffer
    for (uint i = 0; i < level.enemies.size()-1; i++) {
        for (uint j = i+1; j < level.enemies.size(); j++) {
            if (level.enemies[i].distance < level.enemies[j].distance) {
                Enemy temp = level.enemies[i];
                level.enemies[i] = level.enemies[j];
                level.enemies[j] = temp;
            }
        }
    }

    for (uint i = 0; i < level.enemies.size(); i++) {
        Enemy enemy = level.enemies[i];

        double heightAndWidth = ((Constants::DISTANCE_TO_PROJECTION*enemy.sprite.getTexture()->getSize().x)/enemy.distance);

        double projectedSliceHeight = heightAndWidth;
        double projectedSliceWidth = heightAndWidth;

        int startTexture = (int) ((Constants::WIDTH_2) - (projectedSliceWidth/2)+(enemy.angle*Constants::WIDTH/Constants::FOV_D));
        int endTexture = startTexture + (int) (projectedSliceWidth);
        double divisor = (double) (endTexture - startTexture) / enemy.sprite.getTexture()->getSize().x;

        double counter = 0;
        if (startTexture < 0) counter = abs(startTexture);

        for (int i = 0; i < Constants::WIDTH; i++) {
            if (i >= startTexture && i <= endTexture) {
                if (enemy.distance < level.zBuffer[i]) {
                    int sub = (int) (counter / (divisor + 0.01));

                    double val = enemy.distance / fogValue;
                    if (val > 1.0) val = 1.0;
                    val *= 255;
                    int v = 255-val;
                    sf::Color color(v, v, v);
                    enemy.sprite.setColor(color);

                    enemy.sprite.setTextureRect(sf::IntRect(sub, 0, 1, enemy.sprite.getLocalBounds().height));
                    sf::Vector2f targetSize(1.0f, projectedSliceHeight);
                    enemy.sprite.setScale(targetSize.x/enemy.sprite.getLocalBounds().width, targetSize.y/enemy.sprite.getLocalBounds().height);
                    enemy.sprite.setPosition(i, (Constants::HEIGHT_2)-(projectedSliceHeight/2.0));
                    window.draw(enemy.sprite);
                }
                counter++;
            }
        }

    }
}

void Render::drawWeapon(sf::RenderWindow& window, Player& player) {
    if (player.attacking) {
        if (Update::weaponOffset < 8) {
            Texture::weaponSprite.setTextureRect(sf::IntRect(0, 0, 128, Texture::weaponSprite.getLocalBounds().height));
        } else {
            Texture::weaponSprite.setTextureRect(sf::IntRect(128, 0, 128, Texture::weaponSprite.getLocalBounds().height));
        }
        Texture::weaponSprite.setScale(2.5, 2.5);
        Texture::weaponSprite.setPosition(Constants::WIDTH_2-(Texture::weaponSprite.getGlobalBounds().width/2), Constants::HEIGHT-Texture::weaponSprite.getGlobalBounds().height+Update::weaponOffset);
        window.draw(Texture::weaponSprite);
    }
}
