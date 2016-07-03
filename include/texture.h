#ifndef __TEXTURE_H_INCLUDED__
#define __TEXTURE_H_INCLUDED__

#include <SFML/Graphics.hpp>

namespace Texture {
    extern sf::Text titleText;
    extern sf::Font titleTextFont;
    extern sf::Texture wallTexture;
    extern sf::Sprite wallTextureSprite;

    void initialize();
};

#endif
