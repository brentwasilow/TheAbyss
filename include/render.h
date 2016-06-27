#ifndef __RENDER_H_INCLUDED__
#define __RENDER_H_INCLUDED__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace Render {
    static sf::Music titleSong;
    void init(sf::RenderWindow&);
    void drawTitleScreen(sf::RenderWindow&);
    void drawBackground(sf::RenderWindow&);
};

#endif
