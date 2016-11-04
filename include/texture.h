#ifndef __TEXTURE_H_INCLUDED__
#define __TEXTURE_H_INCLUDED__

#include <SFML/Graphics.hpp>

namespace Texture {
    extern sf::Text titleText;
    extern sf::Font titleTextFont;
    extern sf::Texture wallTexture;
    extern sf::Sprite wallTextureSprite;
    extern sf::Texture sprites;
    extern sf::Texture enemy1;
    extern sf::Texture enemy2;
    extern sf::Texture enemy3;
    extern sf::Texture enemy4;
    extern sf::Texture weapon;
    extern sf::Sprite weaponSprite;

    extern sf::Image spritesImage;
    extern sf::Image wallTextureImage;

    void initialize();
};

#endif
