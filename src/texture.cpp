#include "texture.h"
#include "constants.h"
#include <stdlib.h>

sf::Text Texture::titleText;
sf::Font Texture::titleTextFont;
sf::Texture Texture::wallTexture;

sf::Image Texture::wallTextureImage;
sf::Image Texture::spritesImage;

sf::Sprite Texture::wallTextureSprite;
sf::Texture Texture::sprites;
sf::Texture Texture::enemy1;
sf::Texture Texture::enemy2;
sf::Texture Texture::enemy3;
sf::Texture Texture::enemy4;
sf::Texture Texture::weapon;
sf::Sprite Texture::weaponSprite;

void Texture::initialize() {
    // load title text font
    if (!Texture::titleTextFont.loadFromFile("res/titleTextFont.ttf")) {
        exit(EXIT_FAILURE);
    }

    // create title text texture
    Texture::titleText.setFont(Texture::titleTextFont);
    Texture::titleText.setString("The Abyss");
    Texture::titleText.setCharacterSize(50);
    Texture::titleText.setColor(sf::Color::White);

    // calculate centering for title text
    sf::FloatRect titleTextRect = Texture::titleText.getLocalBounds();
    Texture::titleText.setOrigin(titleTextRect.left + titleTextRect.width/2.0f,
                        titleTextRect.top  + titleTextRect.height/2.0f);
    Texture::titleText.setPosition(sf::Vector2f(Constants::WIDTH_2, Constants::HEIGHT_2));

    // load wall textures
    if (!Texture::wallTexture.loadFromFile("res/wallTexture.png")) {
        exit(EXIT_FAILURE);
    }
    // store the wall texture in a sprite
    Texture::wallTextureSprite.setTexture(Texture::wallTexture);

//keep this one
    if (!Texture::wallTextureImage.loadFromFile("res/wallTexture.png")) {
        exit(EXIT_FAILURE);
    }
///
////
    if (!Texture::spritesImage.loadFromFile("res/sprites.png")) {
        exit(EXIT_FAILURE);
    }
////

    // load enemy1 texture
    if (!Texture::enemy1.loadFromFile("res/enemy1.png")) {
        exit(EXIT_FAILURE);
    }

    if (!Texture::enemy2.loadFromFile("res/enemy2.png")) {
        exit(EXIT_FAILURE);
    }

    if (!Texture::enemy3.loadFromFile("res/enemy3.png")) {
        exit(EXIT_FAILURE);
    }

    if (!Texture::enemy4.loadFromFile("res/enemy4.png")) {
        exit(EXIT_FAILURE);
    }

    if (!Texture::sprites.loadFromFile("res/sprites.png")) {
        exit(EXIT_FAILURE);
    }

    // load attack texture
    if (!Texture::weapon.loadFromFile("res/attack.png")) {
        exit(EXIT_FAILURE);
    }
    Texture::weaponSprite.setTexture(Texture::weapon);

}
