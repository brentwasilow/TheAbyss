#include "render.h"
#include "constants.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include "texture.h"
#include "update.h"

double verticalIntersection(double, Player&, Level&);
double horizontalIntersection(double, Player&, Level&);
double bound(double);

// file-only global variables
static sf::Uint8* pixels = new sf::Uint8[Constants::PIXEL_BUFFER_SIZE];
static sf::Texture pixelBufferTexture;
static sf::Sprite pixelBufferSprite;

static int textureOffsetVertical;
static int textureOffsetHorizontal;
static int subimageOffsetVerticalX;
static int subimageOffsetVerticalY;
static int subimageOffsetHorizontalX;
static int subimageOffsetHorizontalY;

// file-only global constant variables
static const double fogValue = (16 * Constants::TILE_SIZE);
static const int floorTexOffsetX = ((5) * (Constants::TILE_SIZE + 1));
static const int floorTexOffsetY = ((4) * (Constants::TILE_SIZE + 1));

double bound(double angle) {
    if (angle < 0.0) angle += 360.0;
    else if (angle >= 360.0) angle -= 360.0;
    return angle;
}

sf::Color determineDepthShade(double distance) {
    double depthColor = distance / fogValue;
    if (depthColor > 1.0) depthColor = 1.0;
    depthColor *= 255;
    int col = (255 - depthColor);
    sf::Color depthShade(col, col, col);
    return depthShade;
}

void Render::setup(sf::RenderWindow& window) {
    // context settings
    sf::ContextSettings settings;
    settings.antialiasingLevel = Constants::ANTI_ALIASING_LEVEL;

    // create the window with context settings
    window.create(sf::VideoMode(Constants::WIDTH, Constants::HEIGHT), Constants::TITLE, sf::Style::Default, settings);

    // center window
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i((mode.width/2)-Constants::WIDTH_2, (mode.height/2)-(Constants::HEIGHT_2)));

    // create pixel buffer texture and attach to sprite
    pixelBufferTexture.create(Constants::WIDTH, Constants::HEIGHT);
    pixelBufferSprite.setTexture(pixelBufferTexture);
}

void Render::drawTitleScreen(sf::RenderWindow& window) {
    window.draw(Texture::titleText);
}

void Render::drawMap(sf::RenderWindow& window, Player& player, Level& level) {
    double numerator = player.height * Constants::DISTANCE_TO_PROJECTION;
    double angle;

    double distance;
    double verticalDistance;
    double horizontalDistance;
    double correctDistance;
    double projectedSliceHeight;
    double wallScale;

    int textureOffset;
    int subimageOffsetX;
    int subimageOffsetY;

    for (int x = 0; x < Constants::WIDTH; x++) {
        double ang = atan(double(x-Constants::WIDTH_2) / Constants::DISTANCE_TO_PROJECTION);
        angle = bound(player.angle+(ang*180.0/M_PI));
        verticalDistance = verticalIntersection(angle, player, level);
        horizontalDistance = horizontalIntersection(angle, player, level);

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
        correctDistance = distance * cos(ang);
        projectedSliceHeight = Constants::DISTANCE_TO_PROJECTION_TILE_SIZE / correctDistance;
        wallScale = ceil(projectedSliceHeight) / Constants::TILE_SIZE;
        sf::Color wallDepthShade = determineDepthShade(distance);

        level.zBuffer[x] = distance;

        int wallCounter = 0;

        int wallTop = (Constants::HEIGHT_2-player.offset) - int(ceil(projectedSliceHeight)/2);
        if (wallTop < 0) {
            wallCounter += -wallTop;
            wallTop = 0;
        }

        int wallBottom = (Constants::HEIGHT_2-player.offset) + int(ceil(projectedSliceHeight)/2);
        if (wallBottom > Constants::HEIGHT) {
            wallBottom = Constants::HEIGHT;
        }

        int index = (wallTop * Constants::WIDTH + x) * 4;
        int indexAdd = (Constants::WIDTH * 4);
        for (int y = wallTop; y <= wallBottom; y++) {
            sf::Color wallColor = Texture::wallTextureImage.getPixel(subimageOffsetX+textureOffset, subimageOffsetY+int(wallCounter/wallScale));
            wallColor *= wallDepthShade;

            pixels[index] = wallColor.r;
            pixels[index+1] = wallColor.g;
            pixels[index+2] = wallColor.b;
            pixels[index+3] = wallColor.a;

            wallCounter++;
            index += indexAdd;
        }
        int counter = 0;

        float correction = 1.0f/cos(ang);
        float xComponent = cos(angle*M_PI/180);
        float yComponent = -sin(angle*M_PI/180);
        int denom = int(ceil(projectedSliceHeight)/2);

        for (int y = wallBottom+1; y <= Constants::HEIGHT; y++) {
            float wallDistance = numerator / denom;
            float correctWallDistance = wallDistance * correction;
            denom++;

            float changeInX = (xComponent * correctWallDistance) + player.x;
            float changeInY = (yComponent * correctWallDistance) + player.y;

            int texX = int(changeInX) & 63;
            int texY = int(changeInY) & 63;

            // depth shading
            sf::Color floorDepthShade = determineDepthShade(correctWallDistance);

            sf::Color color = Texture::wallTextureImage.getPixel(floorTexOffsetX+texX, floorTexOffsetY+texY);
            color *= floorDepthShade;

            int floorIndex = (y * Constants::WIDTH + x) * 4;

            pixels[floorIndex] = color.r;
            pixels[floorIndex + 1] = color.g;
            pixels[floorIndex + 2] = color.b;
            pixels[floorIndex + 3] = color.a;

            counter++;
        }
        //numerator = player.height * Constants::DISTANCE_TO_PROJECTION;
        denom = int(ceil(projectedSliceHeight)/2);
        for (int y = wallTop-1; y >= 0; y--) {
            float wallDistance = numerator / denom;
            float correctWallDistance = wallDistance * correction;
            denom++;

            float changeInX = (xComponent * correctWallDistance) + player.x;
            float changeInY = (yComponent * correctWallDistance) + player.y;

            int texX = int(changeInX) & 63;
            int texY = int(changeInY) & 63;

            // depth shading
            sf::Color ceilDepthShade = determineDepthShade(correctWallDistance);

            sf::Color color = Texture::wallTextureImage.getPixel(floorTexOffsetX+texX, floorTexOffsetY+texY);
            color *= ceilDepthShade;

            int ceilingIndex = (y * Constants::WIDTH + x) * 4;

            pixels[ceilingIndex] = color.r;
            pixels[ceilingIndex + 1] = color.g;
            pixels[ceilingIndex + 2] = color.b;
            pixels[ceilingIndex + 3] = color.a;

        }
    }
    pixelBufferTexture.update(pixels);
    window.draw(pixelBufferSprite);
}

double verticalIntersection(double angle, Player& player, Level& level) {
    double verticalX;
    double verticalY;
    double dx;
    double dy;
    double t = tan(angle * M_PI / 180.0);

    if (angle == 90.0 || angle == 270.0) return 100000000.0;

    if (angle > 90.0 && angle < 270.0) {
        verticalX = floor(player.x / Constants::TILE_SIZE) * Constants::TILE_SIZE;
        dx = -Constants::TILE_SIZE;
        verticalY = player.y + (player.x - verticalX) * t;
        dy = t * Constants::TILE_SIZE;
        verticalX--;
    } else {
        verticalX = (floor(player.x / Constants::TILE_SIZE) * Constants::TILE_SIZE) + Constants::TILE_SIZE;
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
    textureOffsetVertical = int(verticalY)-(row * Constants::TILE_SIZE);

    // determine wall type
    if (level.map[row][column] == Constants::BRICK) {
        subimageOffsetVerticalX = 0*65;
        subimageOffsetVerticalY = 2*65;
    } else if (level.map[row][column] == Constants::DOOR) {
        subimageOffsetVerticalX = 2*65;
        subimageOffsetVerticalY = 6*65;
        double tempDistance = (int(verticalX)+(dx/2.0)-int(player.x))*(int(verticalX)+(dx/2.0)-int(player.x))+
                              (int(verticalY)+(dy/2.0)-int(player.y))*(int(verticalY)+(dy/2.0)-int(player.y));
        textureOffsetVertical = int(verticalY)+(dy/2.0)-(row*Constants::TILE_SIZE);
        return sqrt(tempDistance);
    }

    // check if wall is next to a door so we can render the adjoining door wall correctly
    // also perform short circuit evaluation to ensure that array index is not out of bounds
    if (column+1 < int(level.map[row].size()) && level.map[row][column+1] == Constants::DOOR) {
        subimageOffsetVerticalX = 5*65;
        subimageOffsetVerticalY = 6*65;
    }
    if (column-1 >= 0 && level.map[row][column-1] == Constants::DOOR) {
        subimageOffsetVerticalX = 5*65;
        subimageOffsetVerticalY = 6*65;
    }
    double tempDistance = (int(verticalX)-int(player.x))*(int(verticalX)-int(player.x))+
                          (int(verticalY)-int(player.y))*(int(verticalY)-int(player.y));
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
        horizontalY = (floor(player.y/Constants::TILE_SIZE) * Constants::TILE_SIZE);
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
        horizontalY = (floor(player.y/Constants::TILE_SIZE)*Constants::TILE_SIZE) + Constants::TILE_SIZE;
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

    // determine wall type
    if (level.map[row][column] == Constants::BRICK) {
        subimageOffsetHorizontalX = 0*65;
        subimageOffsetHorizontalY = 2*65;
    } else if (level.map[row][column] == Constants::DOOR) {
        subimageOffsetHorizontalX = 2*65;
        subimageOffsetHorizontalY = 6*65;
        double tempDistance = (int(horizontalX)+(dx/2.0)-int(player.x))*(int(horizontalX)+(dx/2.0)-int(player.x))+
                              (int(horizontalY)+(dy/2.0)-int(player.y))*(int(horizontalY)+(dy/2.0)-int(player.y));
        textureOffsetHorizontal = int(horizontalX)+(dx/2.0)-(column * Constants::TILE_SIZE);
        return sqrt(tempDistance);
    }

    // check if wall is next to a door so we can render the adjoining door wall correctly
    // also perform short circuit evaluation to ensure that array index is not out of bounds
    if (row+1 < int(level.map.size()) && level.map[row+1][column] == Constants::DOOR) {
        subimageOffsetHorizontalX = 5 * 65;
        subimageOffsetHorizontalY = 6 * 65;
    }
    if (row-1 >= 0 && level.map[row-1][column] == Constants::DOOR) {
        subimageOffsetHorizontalX = 5*65;
        subimageOffsetHorizontalY = 6*65;
    }

    double tempDistance = (int(horizontalX)-int(player.x))*(int(horizontalX)-int(player.x))+
                          (int(horizontalY)-int(player.y))*(int(horizontalY)-int(player.y));
    return sqrt(tempDistance);
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

void Render::cleanup() {
    delete [] pixels;
    pixels = NULL;
}

